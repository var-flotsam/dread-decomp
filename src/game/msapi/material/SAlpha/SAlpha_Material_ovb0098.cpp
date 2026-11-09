// From unit 229 of tools/tu_map.py (0x71001a12f4-0x71001a9f3c, msapi::material::SAlpha and the
// unit's other material types): a material's reset, its named value blocks (vtable unk_7101a1e4d8),
// its passes (vtable unk_7101a1e358) and their texture stages (vtable unk_7101a1e328), the render
// state and uniforms it applies, reading a reflected object field by field, and the unit's static
// initialiser. Names and layouts are placeholders (see src/game/msapi/material/SAlpha.cpp).
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;

// ---- The element types ----

// An element destroyed through its vtable's slot 1 (the complete destructor).
struct UnkMatPolyElem_ovb0098 {
    virtual void vf_0x0();
    virtual ~UnkMatPolyElem_ovb0098();
};
// A named value block (0x38 bytes) and a texture stage (0x88 bytes), as their owners destroy them.
struct UnkMatPolyValue_ovb0098 : UnkMatPolyElem_ovb0098 {
    u8 pad_0x8[0x30];
};
struct UnkMatPolyStage_ovb0098 : UnkMatPolyElem_ovb0098 {
    u8 pad_0x8[0x80];
};
// A pass (0x60 bytes) as the material destroys it.
struct UnkMatPolyPass_ovb0098 : UnkMatPolyElem_ovb0098 {
    u8 pad_0x8[0x58];
};

// A counted reference to a resource (base/UnkCountedResource.h): copies start out empty and then
// take the other's resource.
struct UnkCountedPtr_ovb0098 {
    UnkCountedResource* ptr = nullptr;

    UnkCountedPtr_ovb0098() = default;
    UnkCountedPtr_ovb0098(const UnkCountedPtr_ovb0098& other) { *this = other; }
    UnkCountedPtr_ovb0098& operator=(const UnkCountedPtr_ovb0098& other) {
        UnkCountedResource* r = other.ptr;
        if (ptr != r) {
            if (r)
                ++r->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = r;
        }
        return *this;
    }
};

// Drops the reference in `slot` and clears it.
static inline void Release(UnkCountedResource*& slot) {
    if (UnkCountedResource* r = slot) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
        slot = nullptr;
    }
}

using UnkPathId_ovb0098 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The stage's vtable (reached through the GOT).
extern char unk_7101a1e328[];
// The base destructor of the unit's polymorphic elements.
void fn_710002384c(void* self);

struct UnkMatStageData_ovb0098 {
    u64 field_0x30[6];
};

// A texture stage of a pass (0x88 bytes): three string ids, an id, a texture path, its parameters
// and the counted texture resource.
struct UnkMatStage_ovb0098 {
    char* vtable;
    base::UnkStrIdMember field_0x8;
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    u32 field_0x20;
    UnkPathId_ovb0098 field_0x28;
    UnkMatStageData_ovb0098 field_0x30;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u64 field_0x78;
    UnkCountedPtr_ovb0098 resource;  // 0x80

    UnkMatStage_ovb0098(UnkMatStage_ovb0098&& o)
        : vtable(unk_7101a1e328 + 0x10),
          field_0x8(static_cast<base::UnkStrIdMember&&>(o.field_0x8)),
          field_0x10(static_cast<base::UnkStrIdMember&&>(o.field_0x10)),
          field_0x18(static_cast<base::UnkStrIdMember&&>(o.field_0x18)),
          field_0x20(o.field_0x20),
          field_0x28(static_cast<UnkPathId_ovb0098&&>(o.field_0x28)),
          field_0x30(o.field_0x30),
          field_0x60(o.field_0x60),
          field_0x64(o.field_0x64),
          field_0x68(o.field_0x68),
          field_0x6c(o.field_0x6c),
          field_0x70(o.field_0x70),
          field_0x78(o.field_0x78),
          resource(o.resource) {}
    // Destroyed through the vtable's slot 1.
    void Destroy() { reinterpret_cast<void (*)(UnkMatStage_ovb0098*)>(reinterpret_cast<void**>(vtable)[1])(this); }
};
static_assert(sizeof(UnkMatStage_ovb0098) == 0x88, "UnkMatStage_ovb0098 placeholder layout");

// ---- fn_71001a8f48: the stage's destructor ----

void fn_71001a8f48(UnkMatStage_ovb0098* self) {
    self->vtable = unk_7101a1e328 + 0x10;
    if (UnkCountedResource* r = self->resource.ptr) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    using Id = base::UnkStrIdMember;
    self->field_0x28.~UnkPathId_ovb0098();
    self->field_0x18.~Id();
    self->field_0x10.~Id();
    self->field_0x8.~Id();
    fn_710002384c(self);
}

// ---- fn_71001a7a2c: growing a pass's stage array ----

struct UnkMatStageVector_ovb0098 : base::UnkVector<UnkMatStage_ovb0098> {};

// Grows the array to hold n stages, moving them over and destroying the old ones.
void fn_71001a7a2c(UnkMatStageVector_ovb0098* v, u32 n) {
    if (v->capacity < n) {
        UnkMatStage_ovb0098* p =
            static_cast<UnkMatStage_ovb0098*>(v->allocator->vf_0x40((u64)n * sizeof(UnkMatStage_ovb0098), 8, v->flags));
        if (v->data) {
            UnkMatStage_ovb0098* src = v->data;
            UnkMatStage_ovb0098* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
                new (dst) UnkMatStage_ovb0098(static_cast<UnkMatStage_ovb0098&&>(*src));
                src->Destroy();
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = p;
        v->capacity = n;
    }
}

// ---- fn_71001a90fc: the pass's destructor ----

// The pass's vtable (reached through the GOT).
extern char unk_7101a1e358[];

struct UnkMatPass_ovb0098 {
    char* vtable;
    u64 field_0x8;
    base::UnkVector<UnkMatPolyValue_ovb0098> values;  // 0x10
    base::UnkVector<UnkMatPolyStage_ovb0098> stages;  // 0x38
};
static_assert(sizeof(UnkMatPass_ovb0098) == 0x60, "UnkMatPass_ovb0098 placeholder layout");

void fn_71001a90fc(UnkMatPass_ovb0098* self) {
    self->vtable = unk_7101a1e358 + 0x10;
    self->stages.Free();
    self->values.Free();
    fn_710002384c(self);
}

// ---- fn_71001a9bd0: resizing a value block array ----

// The value block's vtable (reached through the GOT); its slot 1 is the complete destructor.
extern char unk_7101a1e4d8[];

// The element's plain part (copied as a whole).
struct UnkMaterialValuesData {
    u8 field_0x10;
    s32 field_0x14;  // the number of values
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    void* field_0x30;  // the values
};

struct UnkMaterialValues {
    char* vtable;
    CStrId id;
    UnkMaterialValuesData d;

    UnkMaterialValues(const UnkMaterialValues& other) : vtable(unk_7101a1e4d8 + 0x10), id(other.id), d(other.d) {}

    void Destroy() { reinterpret_cast<void (*)(UnkMaterialValues*)>(reinterpret_cast<void**>(vtable)[1])(this); }
};
static_assert(sizeof(UnkMaterialValues) == 0x38, "UnkMaterialValues placeholder layout");

// The array (base/UnkVector.h's layout), as src/game/msapi/material/SAlpha.cpp defines it.
struct UnkMaterialValuesVector : base::UnkVector<UnkMaterialValues> {
    // Grows the array to hold n elements, copying them over and destroying the old ones.
    void Reserve(u32 n) {
        if (capacity < n) {
            UnkMaterialValues* p =
                static_cast<UnkMaterialValues*>(allocator->vf_0x40((u64)n * sizeof(UnkMaterialValues), 8, flags));
            if (data) {
                UnkMaterialValues* src = data;
                UnkMaterialValues* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    new (dst) UnkMaterialValues(*src);
                    src->Destroy();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    // The last element (the first when there is none).
    UnkMaterialValues* Back() {
        u32 n = count;
        UnkMaterialValues* end = data + n;
        return n ? end - 1 : end;
    }
};

// The value block's default constructor.
void fn_71001a97fc(UnkMaterialValues* self);

// Grows the array to n elements, default-constructing the new ones.
void fn_71001a9bd0(UnkMaterialValuesVector* v, u32 n) {
    v->Reserve(n);
    for (u32 i = v->count; i < n; ++i)
        fn_71001a97fc(&v->data[i]);
    v->count = n;
}

// ---- fn_71001a6418: emptying a material ----

struct UnkRegistry_710014c790;
extern UnkRegistry_710014c790* unk_710b372d68;  // the registry (reached through the GOT)
void fn_7100169f34(UnkRegistry_710014c790* registry, base::UnkStrIdMember* key);

// The material as its reset sees it: its resource at 8, value blocks at 0x330, passes at 0x358,
// a second resource at 0x388, a registration (handle and key) at 0x390 and the ids at 0x3a8.
struct UnkMaterialReset_ovb0098 {
    void* vtable;
    UnkCountedResource* resource;  // 0x8
    u8 pad_0x10[0x320];
    base::UnkVector<UnkMatPolyValue_ovb0098> values;  // 0x330
    base::UnkVector<UnkMatPolyPass_ovb0098> passes;   // 0x358
    UnkCountedResource* field_0x380;
    UnkCountedResource* field_0x388;
    void* handle;              // 0x390
    base::UnkStrIdMember key;  // 0x398
    u8 pad_0x3a0[0x8];
    base::UnkVector<CStrId> ids;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkMaterialReset_ovb0098, ids) == 0x3a8, "UnkMaterialReset_ovb0098 layout");

// Unregisters the material, drops its resources and destroys its value blocks, passes and ids.
void fn_71001a6418(UnkMaterialReset_ovb0098* self) {
    if (self->handle) {
        fn_7100169f34(unk_710b372d68, &self->key);
        self->handle = nullptr;
    }
    Release(self->field_0x388);
    self->values.Clear();
    self->passes.Clear();
    Release(self->resource);
    for (u64 i = 0; i < self->ids.count; ++i)
        self->ids.data[i].Reset();
    self->ids.count = 0;
}

// ---- fn_71001a76cc: the step after reading a material ----

// The material as fn_71001a6644 (src/game/msapi/material/SAlpha.cpp) reads it: the passes at
// 0x358 and the ids at 0x3a8.
struct UnkMaterial {
    u8 pad_0x0[0x358];
    base::UnkVector<UnkMatPass_ovb0098> passes;
    u8 pad_0x380[0x28];
    base::UnkVector<CStrId> ids;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkMaterial, ids) == 0x3a8, "UnkMaterial placeholder layout");

void fn_71001a836c(UnkMaterial* self, base::UnkVector<u64>* list);

// Empties the ids, then runs fn_71001a836c over each pass's stages (fn_71001a6644's last steps).
void fn_71001a76cc(UnkMaterial* self) {
    for (u64 i = 0; i < self->ids.count; ++i)
        self->ids.data[i].Reset();
    self->ids.count = 0;
    for (UnkMatPass_ovb0098* p = self->passes.data; p != self->passes.data + self->passes.count; ++p)
        fn_71001a836c(self, reinterpret_cast<base::UnkVector<u64>*>(&p->stages));
}

// ---- fn_71001a7f50: setting a value block list's uniforms ----

void fn_710015e1f0(void* state, u32 stage, u64 location, u32 count, const f32* values);
// The integer counterpart of fn_710015e1f0.
void fn_710015e110(void* state, u32 stage, u64 location, u32 count, const s32* values);
// The program's uniform state (null when it has none).
void* fn_71001aa028(void* program);

// A value block as a uniform: its type ('f' or 'i'), its count, its location in each of the three
// stages (0 when absent) and the values.
struct UnkMatUniform_ovb0098 {
    void* vtable;
    u64 id;
    u8 type;          // 0x10
    u32 count;        // 0x14
    u64 location[3];  // 0x18
    void* values;     // 0x30
};

struct UnkMatUniforms_ovb0098 {
    UnkMatUniform_ovb0098* data;
    u32 count;
};

struct UnkMatProgramOwner_ovb0098 {
    u8 pad_0x0[0x390];
    void* program;  // 0x390
};

static inline void SetFloats(void* state, u32 stage, const UnkMatUniform_ovb0098* u) {
    fn_710015e1f0(state, stage, u->location[stage], u->count, static_cast<const f32*>(u->values));
}

static inline void SetInts(void* state, u32 stage, const UnkMatUniform_ovb0098* u) {
    fn_710015e110(state, stage, u->location[stage], u->count, static_cast<const s32*>(u->values));
}

// Sets every block of the list as a uniform of each stage that has it, on the material's program.
__attribute__((noinline)) void fn_71001a7f50(UnkMatProgramOwner_ovb0098* self, UnkMatUniforms_ovb0098* list) {
    if (!self->program)
        return;
    UnkMatUniform_ovb0098* it = list->data;
    void* state = fn_71001aa028(self->program);
    if (!state)
        return;
    for (; it != list->data + list->count; ++it) {
        if (it->location[0]) {
            switch (it->type) {
            case 'f':
                SetFloats(state, 0, it);
                break;
            case 'i':
                SetInts(state, 0, it);
                break;
            }
        }
        if (it->location[1]) {
            switch (it->type) {
            case 'i':
                SetInts(state, 1, it);
                break;
            case 'f':
                SetFloats(state, 1, it);
                break;
            }
        }
        if (it->location[2]) {
            switch (it->type) {
            case 'i':
                SetInts(state, 2, it);
                break;
            case 'f':
                SetFloats(state, 2, it);
                break;
            }
        }
    }
}

// ---- fn_71001a7bfc: applying the material's render state ----

void fn_710015a3e0(u32 value);
void fn_710015a500(u32 a, u32 b);
void fn_710015a680(u32 value);
void fn_710015ab00(u32 value);
void fn_710015aa80(u32 value);
void fn_710015a860(u32 value);
void fn_710015aa20(u32 value);
void fn_710015a8c0(u32 value);

// The state fn_710015acf0 sets in one go (bit 3).
struct UnkBlendDesc_ovb0098 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 field_0x18;
    u8 field_0x19;
};
void fn_710015acf0(const UnkBlendDesc_ovb0098* desc);

struct UnkMatStateOwner_ovb0098;
void fn_71001a7dc8(UnkMatStateOwner_ovb0098* self);
void fn_71001a80b4(UnkMatStateOwner_ovb0098* self, u32 id, void* stages);

struct UnkMatStatePass_ovb0098 {
    u8 pad_0x0[0x8];
    u32 id;  // 0x8
    u8 pad_0xc[0x4];
    UnkMatUniforms_ovb0098 uniforms;  // 0x10
    u8 pad_0x20[0x18];
    u8 stages[0x28];  // 0x38
};
static_assert(sizeof(UnkMatStatePass_ovb0098) == 0x60, "UnkMatStatePass_ovb0098 placeholder layout");

struct UnkMatStateOwner_ovb0098 {
    u8 pad_0x0[0x30];
    u8 field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u8 pad_0x40[0x8];
    u32 field_0x48;
    u8 pad_0x4c[0xc];
    u8 field_0x58;
    u8 pad_0x59[0x3];
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u8 pad_0x70[0x4];
    u32 field_0x74;
    u8 pad_0x78[0x20];
    u32 field_0x98;
    u8 pad_0x9c[0xc];
    u8 field_0xa8;
    u8 field_0xa9;
    u8 pad_0xaa[0x2];
    u32 field_0xac;
    u8 pad_0xb0[0x280];
    UnkMatUniforms_ovb0098 uniforms;  // 0x330
    u8 pad_0x340[0x18];
    UnkMatStatePass_ovb0098* passes;  // 0x358
    u32 pass_count;
};
static_assert(__builtin_offsetof(UnkMatStateOwner_ovb0098, passes) == 0x358, "UnkMatStateOwner_ovb0098 layout");

static inline UnkMatProgramOwner_ovb0098* AsProgramOwner(UnkMatStateOwner_ovb0098* self) {
    return reinterpret_cast<UnkMatProgramOwner_ovb0098*>(self);
}

// Applies the parts of the state that `flags` names; bits 6 and 9 also go through each pass.
void fn_71001a7bfc(UnkMatStateOwner_ovb0098* self, u32 flags) {
    if (flags & 2) {
        fn_710015a3e0(self->field_0x30);
        if (self->field_0x30) {
            fn_710015a680(self->field_0x34);
            fn_710015a500(self->field_0x38, self->field_0x3c);
        }
    }
    if (flags & 4)
        fn_710015ab00(self->field_0x48);
    if (flags & 0x20)
        fn_710015aa80(self->field_0x98);
    if (flags & 8) {
        UnkBlendDesc_ovb0098 desc;
        desc.field_0x19 = 1;
        desc.field_0x18 = self->field_0x58;
        desc.field_0x4 = self->field_0x6c;
        desc.field_0x0 = self->field_0x64;
        desc.field_0x8 = self->field_0x68;
        desc.field_0xc = self->field_0x74;
        desc.field_0x14 = self->field_0x60;
        desc.field_0x10 = self->field_0x5c;
        fn_710015acf0(&desc);
    }
    if (flags & 0x10) {
        fn_710015a860(self->field_0xa8);
        fn_710015aa20(self->field_0xa9);
        if (self->field_0xa8)
            fn_710015a8c0(self->field_0xac);
    }
    if (flags & 0x80)
        fn_71001a7dc8(self);
    if (flags & 0x40)
        fn_71001a7f50(AsProgramOwner(self), &self->uniforms);
    if (flags & ~0x13fu) {
        for (UnkMatStatePass_ovb0098* p = self->passes; p != self->passes + self->pass_count; ++p) {
            if (flags & 0x40)
                fn_71001a7f50(AsProgramOwner(self), &p->uniforms);
            if (flags & 0x200)
                fn_71001a80b4(self, p->id, p->stages);
        }
    }
}

// ---- fn_71001a69a0: reading a reflected object's fields ----

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e7a80(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // sets
// The member object a field of `obj` holds.
UnkReflTypedPtr fn_71000e755c(UnkReflField* field, const UnkReflTypedPtr* obj);
const base::reflection::UnkReflAttrKey& fn_710000bf80();
// The field's attribute `key` as a value (an empty value when the field has none).
UnkReflValue fn_710000cf10(void* field, const base::reflection::UnkReflAttrKey& key, bool inherited);
void fn_710007ca94(CRntFile*, char*);
// Reads `size` bytes (fn_710007c714 with one element).
u64 fn_710007c70c(CRntFile* file, void* dest, u64 size);

// A field's type: its class (vtable slot 0) and its size.
struct UnkReadFieldType_ovb0098 {
    virtual const CClass* GetClass();
    u8 pad_0x8[0x10];
    u64 size;  // 0x18
};

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkReadField_ovb0098 {
    u8 pad_0x0[0x10];
    UnkReadFieldType_ovb0098* type;
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkReadField_ovb0098) == 0xb0, "UnkReadField_ovb0098 placeholder layout");

struct UnkReadClass_ovb0098 {
    u8 pad_0x0[0xe8];
    UnkReadField_ovb0098* fields;  // 0xe8
    u8 pad_0xf0[0x8];
    u32 count;  // 0xf8
};

struct UnkReadObject_ovb0098 {
    virtual UnkReadClass_ovb0098* GetClass();
};

// A value's type and where it is.
struct UnkReadValue_ovb0098 {
    const CType* type;
    void* value;
};

// Whether the field is read: when it has no such attribute, or the attribute is 1.
static inline bool Readable(UnkReadField_ovb0098* f) {
    UnkReflValue v = fn_710000cf10(f, fn_710000bf80(), true);
    if (!v.type) {
        fn_71000a1c60(&v);
        return true;
    }
    const void* p = v.field_0x14 == 1 || *reinterpret_cast<const u64*>(v.type + 0x18) >= 9 ? v.field_0x0 : &v;
    s32 value = *static_cast<const s32*>(p);
    fn_71000a1c60(&v);
    return value == 1;
}

// Reads the object's fields from the file in order: strings (char const*) as their text, plain
// values, enums and flag sets as their bytes. The first argument is not read.
void fn_71001a69a0(void*, CRntFile* file, void* dst) {
    UnkReadObject_ovb0098* obj = static_cast<UnkReadObject_ovb0098*>(dst);
    UnkReadClass_ovb0098* cls = obj->GetClass();
    for (UnkReadField_ovb0098* f = cls->fields; f != cls->fields + cls->count; ++f) {
        if (!Readable(f))
            continue;
        if (fn_710009d52c(reinterpret_cast<const CType*>(f->type), fn_71000081e8())) {
            char buf[0x200];
            fn_710007ca94(file, buf);
            UnkReflTypedPtr typed;
            typed.type = reinterpret_cast<char*>(obj->GetClass());
            typed.ptr = obj;
            char* text = buf;
            UnkReadValue_ovb0098 value = {fn_71000081e8(), &text};
            fn_71000e7a80(reinterpret_cast<UnkReflField*>(f), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                          reinterpret_cast<UnkReflValueArg*>(&value));
        } else {
            const CClass* k = f->type->GetClass();
            if (k == fn_710009c7b8() || k == fn_710009921c() || k == fn_710009a11c()) {
                // Declared before the object's typed pointer (the original's stack order).
                UnkReflTypedPtr member;
                {
                    UnkReflTypedPtr typed;
                    typed.type = reinterpret_cast<char*>(obj->GetClass());
                    typed.ptr = obj;
                    new (&member) UnkReflTypedPtr(fn_71000e755c(reinterpret_cast<UnkReflField*>(f), &typed));
                }
                fn_710007c70c(file, member.ptr, f->type->size);
            }
        }
    }
}

// ---- fn_71001a9d30: the unit's static initialiser ----

extern UnkTUHeader g_unk_716e3528b0 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_716e352788;  // msapi::material::ETranslucencyType
extern char g_unk_716e3528a0;
extern char g_unk_716e3528a1;
extern char g_unk_716e3528a2;
extern char g_unk_716e3528a3;
extern char g_unk_716e3528a4;
extern char g_unk_716e3528a5;
extern char g_unk_716e3528a6;
extern char g_unk_716e3528a7;
extern char g_unk_716e3528a8;
extern char g_unk_716e3528a9;
extern char g_unk_716e3528aa;

// The unit's class objects (0x138 bytes each), in the order of their getters.
struct UnkMaterialClassObj_ovb0098 {
    u8 data[0x138];
};
extern UnkMaterialClassObj_ovb0098 g_unk_716e352938[10] __attribute__((visibility("hidden")));

// The enum type's constructor and its values (src/clones/clones_00.cpp).
void fn_71001a1680(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71001a12f4(void* e);

// The getters, defined in this unit, return their class object, so only the call remains and the
// object is named directly.
template <auto Getter, int Index>
__attribute__((always_inline)) static inline void AddClass(char* list) {
    Getter();
    fn_710009ec00(list, &g_unk_716e352938[Index]);
}

void fn_71001a9d30() {
    InitHeader(g_unk_716e3528b0);
    InitEnum(reinterpret_cast<UnkReflType*>(&g_unk_716e352788), "msapi::material::ETranslucencyType", fn_71001a1680,
             reinterpret_cast<void (*)()>(fn_71001a12f4));
    fn_710009ec00(&g_unk_716e3528a0, &g_unk_716e352788);
    AddClass<fn_71001a1858, 0>(&g_unk_716e3528a1);  // msapi::material::SAlpha
    AddClass<fn_71001a1db0, 1>(&g_unk_716e3528a2);  // SBlend
    AddClass<fn_71001a23ac, 2>(&g_unk_716e3528a3);  // SCull
    AddClass<fn_71001a2778, 3>(&g_unk_716e3528a4);  // SStencil
    AddClass<fn_71001a3054, 4>(&g_unk_716e3528a5);  // SDepth
    AddClass<fn_71001a3664, 5>(&g_unk_716e3528a6);  // SFill
    AddClass<fn_71001a3a2c, 6>(&g_unk_716e3528a7);  // SConstant
    AddClass<fn_71001a3f24, 7>(&g_unk_716e3528a8);  // SStage
    AddClass<fn_71001a42f0, 8>(&g_unk_716e3528a9);  // STexture
    AddClass<fn_71001a5444, 9>(&g_unk_716e3528aa);  // CMaterial
}

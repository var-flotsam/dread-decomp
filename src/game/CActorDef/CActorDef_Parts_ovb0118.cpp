// CActorDef's unit (tools/tu_map.py): a definition holding resource names (+0x70, an id array), its
// own name (+0x98) and its parts (+0xa0). fn_7100254e84: the destructor (vtable 0x7101a20bf8, base
// fn_710002545c). fn_7100255158: drops the resources the names hold in the actordef factory, has each
// part run its slot 0x30 and, unless +0x50 is set, deletes them. fn_7100256268: reads the flags and
// the names from a file. fn_7100254fdc: the load step: takes the context's name, asks the parts
// (slot 0x20) and releases the resources the names load that nothing else holds. Names and layouts
// are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/core/CDefinition.h"
#include "base/global/CRntFile.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2729.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a20bf8[];  // the class's vtable

struct UnkActorDefData_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x68];
    base::UnkVector<base::UnkStrIdMember> names;  // 0x70
    base::UnkStrIdMember name;                    // 0x98
    base::UnkVector<void*> items;                 // 0xa0
};
static_assert(__builtin_offsetof(UnkActorDefData_ovb0118, items) == 0xa0, "UnkActorDefData_ovb0118 layout");

// Typed as src/game/CCharClass/CCharClass_Paths_ovb0066.cpp declares it.
void fn_7100254e84(void* object) {
    UnkActorDefData_ovb0118* self = static_cast<UnkActorDefData_ovb0118*>(object);
    self->vtable = g_unk_7101a20bf8 + 2;
    self->items.Free();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    self->names.Free();
    fn_710002545c(self);
}

struct UnkActorDefFactory;
struct UnkGuiDef_b52;
extern UnkActorDefFactory* g_unk_71726a0220;

// The definition's parts, created from the names.
struct UnkActorDefPart_ovb0118 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
};

struct UnkActorDefUser_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x48];
    bool keep_parts;  // 0x50
    u8 pad_0x51[0x1f];
    base::UnkVector<base::UnkStrIdMember> names;     // 0x70
    base::UnkStrIdMember name;                       // 0x98
    base::UnkVector<UnkActorDefPart_ovb0118*> parts;  // 0xa0
};

// Typed as src/game/CCharClass/CCharClass_Paths_ovb0066.cpp declares it.
void fn_7100255158(void* object) {
    UnkActorDefUser_ovb0118* self = static_cast<UnkActorDefUser_ovb0118*>(object);
    for (base::UnkStrIdMember* id = self->names.data, *end = id + self->names.count; id != end; ++id) {
        UnkCountedResource* resource =
            reinterpret_cast<UnkCountedResource*>(fn_7101193e88(g_unk_71726a0220, id));
        if (resource && --resource->ref_count == 0)
            fn_710001737c(resource);
    }
    for (UnkActorDefPart_ovb0118 **part = self->parts.data, **end = part + self->parts.count; part != end; ++part)
        (*part)->vf_0x30();
    if (self->keep_parts)
        return;
    for (UnkActorDefPart_ovb0118 **part = self->parts.data, **end = part + self->parts.count; part != end; ++part) {
        if (*part)
            (*part)->vf_0x10();
    }
    self->parts.count = 0;
}

s64 fn_710007cee4(void* stream, const char** out);  // as src/base/global/CRntFile.cpp defines it

struct UnkActorDefFile_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x58];
    u8 field_0x60;
    u8 pad_0x61[3];
    u32 field_0x64;
    u8 field_0x68;
    u8 field_0x69;
    u8 field_0x6a;
    u8 pad_0x6b[5];
    base::UnkStrIdVec names;  // 0x70
};

// Appends the id, taking it over (the id is left empty). The element is stored untyped: the
// original reloads the count after it.
static inline void PushBackMove(base::UnkStrIdVec* names, CStrId& id) {
    u32 n = names->count;
    u32 c = names->capacity;
    if (c <= n && names->step != 0)
        c = (n / names->step + 1) * names->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    names->Reserve(c);
    __builtin_memcpy(&names->data[names->count], &id.data, 8);
    id.data = &g_unk_7101d47108->empty;
    ++names->count;
}

bool fn_7100256268(UnkActorDefFile_ovb0118* self, base::global::CRntFile* file) {
    fn_710007c714(file, &self->field_0x68, 1, 1);
    fn_710007c714(file, &self->field_0x60, 1, 1);
    fn_710007c714(file, &self->field_0x64, 4, 1);
    fn_710007c714(file, &self->field_0x69, 1, 1);
    fn_710007c714(file, &self->field_0x6a, 1, 1);
    s32 count;
    fn_710007c714(file, &count, 4, 1);
    self->names.Reserve(count);
    for (s32 i = 0; i < count; ++i) {
        const char* name;
        fn_710007cee4(file, &name);
        CStrId id(name, true);
        PushBackMove(&self->names, id);
    }
    return true;
}

void fn_710007c144(UnkHashStream_0x38*);
struct UnkSpawnDefRef_b52;
UnkSpawnDefRef_b52 fn_71011938bc(UnkActorDefFactory* factory, const base::global::CStrId* type, const base::global::CFilePathStrId* path);

// What fn_71011938bc returns, as this unit holds it: the resource is released when nothing else
// holds it (its count is not touched here).
struct UnkActorDefRef_ovb0118 {
    UnkCountedResource* ptr;
    ~UnkActorDefRef_ovb0118() {
        if (ptr && ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

struct UnkActorDefContext_ovb0118 {
    u8 pad_0x0[0x20];
    base::UnkStrIdMember name;  // 0x20
    UnkHashStream_0x38* stream;  // 0x28
};

struct UnkActorDefPart2_ovb0118 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20(struct UnkActorDefLoad_ovb0118* def);
};

struct UnkActorDefLoad_ovb0118 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30(UnkHashStream_0x38* stream);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();
    u8 pad_0x8[0x68];
    base::UnkVector<CStrId> names;  // 0x70
    base::UnkStrIdMember name;      // 0x98
    base::UnkVector<UnkActorDefPart2_ovb0118*> parts;  // 0xa0
};

bool fn_7100254fdc(UnkActorDefLoad_ovb0118* self, UnkActorDefContext_ovb0118* context) {
    if (context->stream && !self->vf_0x30(context->stream))
        return false;
    self->name = context->name;
    bool ok = self->name.data != &g_unk_7101d47108->empty;
    ok &= self->vf_0x48();
    for (UnkActorDefPart2_ovb0118 **part = self->parts.data, **end = part + self->parts.count; part != end; ++part)
        ok &= (*part)->vf_0x20(self);
    if (context->stream)
        fn_710007c144(context->stream);
    using Load = UnkActorDefRef_ovb0118 (*)(UnkActorDefFactory*, const CStrId*, const base::global::CFilePathStrId*);
    for (CStrId *id = self->names.data, *end = id + self->names.count; id != end; ++id)
        reinterpret_cast<Load>(fn_71011938bc)(g_unk_71726a0220, id, &g_unk_7101d470e8);
    return ok;
}

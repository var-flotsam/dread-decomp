// The tunables' registry unit (tools/tu_map.py unit 112; src/base/UnkTunableRegistry.cpp): groups of
// tunables kept in two parallel arrays, a group's creation (fn_710013c2ac appends one under a name;
// fn_710013ef1c builds one by taking another's members over), a tunable's value as text
// (fn_710013ab54), setting a named tunable's field from a value (fn_710013c4b4) and the second tunables
// file's read into the registry (fn_710013af08). Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU112.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkVector;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflTypedPtr;

// ---- the groups

struct UnkTunableGroup;

// What a group lists first: objects whose first word points back at the group.
struct UnkTunableMember {
    UnkTunableGroup* owner;
};

// A group of tunables (0x80 bytes): three lists, each with flags 0xf.
struct UnkTunableGroup {
    UnkVector<UnkTunableMember*> members{0xf};
    UnkVector<void*> field_0x28{0xf};
    UnkVector<void*> field_0x50{0xf};
    bool field_0x78;
};
static_assert(sizeof(UnkTunableGroup) == 0x80, "");

// Builds a group as a copy of another, taking its members over.
void fn_710013ef1c(UnkTunableGroup* self, UnkTunableGroup* other) {
    new (self) UnkTunableGroup;
    self->field_0x78 = other->field_0x78;
    for (UnkTunableMember **p = other->members.data, **end = other->members.data + other->members.count; p != end;
         ++p) {
        UnkTunableMember* member = *p;
        // An untyped store: the original reloads the list's count after it.
        __builtin_memcpy(member, &self, sizeof(self));
        self->members.PushBack(member);
    }
    other->members.count = 0;
    other->field_0x28.count = 0;
    other->field_0x50.count = 0;
}

// A new group, as the appended one is made: zeroed, then the lists set up.
struct UnkNewTunableGroup {
    UnkVector<UnkTunableMember*> members{0xf};
    UnkVector<void*> field_0x28{0xf};
    UnkVector<void*> field_0x50{0xf};
    bool field_0x78 = false;
};
static_assert(sizeof(UnkNewTunableGroup) == 0x80, "");

struct UnkTunableGroupArrays;
void fn_710013ec80(UnkTunableGroupArrays* self, u32 new_capacity);

struct UnkTunableGroupNames {
    UnkNewTunableGroup* groups;
    CStrId* names;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;
};

struct UnkTunableGroup_A1b71;
struct UnkTunableGroupArrays_A1b71;

// Appends a group under `key` and returns it.
UnkTunableGroup_A1b71* fn_710013c2ac(UnkTunableGroupArrays_A1b71* self_, const CStrId* key) {
    auto* self = reinterpret_cast<UnkTunableGroupNames*>(self_);
    u32 n = self->count;
    if (n == self->capacity) {
        fn_710013ec80(reinterpret_cast<UnkTunableGroupArrays*>(self), self->step + n);
        n = self->count;
    }
    // The key is copied as raw bytes, then retained: the original reloads the count after the copy.
    CStrId* k = &self->names[n];
    __builtin_memcpy(k, key, sizeof(*k));
    if (&g_unk_7101d47108->empty != k->data)
        ++k->data->ref_count;
    new (&self->groups[self->count]) UnkNewTunableGroup();
    u32 count = ++self->count;
    return reinterpret_cast<UnkTunableGroup_A1b71*>(count ? &self->groups[count] - 1 : self->groups);
}

// ---- a tunable's field

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkTunableLine {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }

    UnkTunableLine() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    UnkTunableLine& operator=(const CRntString& other) {
        fn_710007eaa0(Str(), &other);
        return *this;
    }
    ~UnkTunableLine() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};
static_assert(sizeof(UnkTunableLine) == 0x148, "");

// A reflected value that is released when it goes out of scope.
struct UnkTunableReflValue : base::reflection::UnkReflValue {
    ~UnkTunableReflValue() { fn_71000a1c60(this); }
};

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkTunableField {
    u8 pad_0x0[0x10];
    const base::reflection::CType* type;
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkTunableField) == 0xb0, "");

struct UnkTunableFieldHandle {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

bool fn_71000e750c(UnkTunableFieldHandle* field);  // whether the field can be read
UnkTunableReflValue fn_71000e76dc(UnkTunableFieldHandle* field, const UnkReflTypedPtr* obj);
UnkTunableLine fn_710013a828(const UnkTunableReflValue* value);  // the value as text

static inline __attribute__((always_inline)) UnkTunableLine EmptyLine() {
    UnkTunableLine line;
    line = g_unk_7101d58d10;
    return line;
}

static inline __attribute__((always_inline)) UnkReflTypedPtr Refined(const UnkReflTypedPtr* obj) {
    char* type = obj->type;
    void* ptr = obj->ptr;
    UnkReflTypedPtr typed;
    typed.type = fn_710009dcd8(type, ptr);
    typed.ptr = ptr;
    return typed;
}

static inline __attribute__((always_inline)) UnkTunableReflValue Read(UnkTunableFieldHandle* field,
                                                                      const UnkReflTypedPtr&& typed) {
    return fn_71000e76dc(field, &typed);
}

UnkTunableLine fn_710013ab54(UnkTunableFieldHandle* field, const UnkReflTypedPtr* obj) {
    if (field->field_0x18 || fn_71000e750c(field)) {
        UnkTunableReflValue value = Read(field, Refined(obj));
        return fn_710013a828(&value);
    }
    return EmptyLine();
}

// ---- setting a named tunable's field

// A type descriptor as the lookups see it: its class (vtable slot 0) and its fields by name hash.
struct UnkTunableType {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0xe0];
    UnkTunableField* fields;  // +0xe8
    u64* keys;                // +0xf0
    u32 count;                // +0xf8

    UnkTunableField* End() { return fields + count; }
    UnkTunableField* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};

// The tunable object a header names: its class is vtable slot 0.
struct UnkTunableObject {
    virtual char* GetClass();
};
struct UnkTunableRegistry;

// The field calls' argument: a value's type and where it is.
struct UnkTunableValue {
    const base::reflection::CType* type;
    void* value;
};
struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
s32 fn_7100024140(const base::reflection::CType* type);  // the type's letter (b, f, i, s, v)

// Sets a tunable's field to a value of the same kind.
void fn_710013c4b4(UnkTunableRegistry* self, const char* name, const char* field_name, UnkTunableValue* value) {
    UnkTunableObject* object = fn_710013c5cc(self, name, true);
    if (!object)
        return;
    auto* type = reinterpret_cast<UnkTunableType*>(object->GetClass());
    u64 hash = fn_7100001570(field_name, strlen(field_name));
    UnkTunableField* field = type->Find(hash);
    if (field == type->End())
        return;
    if (fn_7100024140(field->type) != fn_7100024140(value->type))
        return;
    UnkReflTypedPtr typed;
    typed.type = object->GetClass();
    typed.ptr = object;
    fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                  reinterpret_cast<UnkReflValueArg*>(value));
}

// ---- reading the second tunables file

// The id and path pair (base/UnkStrIdPathPair.h) the file is opened from.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000f750(UnkParseContext* self, void* root);

extern UnkParseCallbacks g_unk_710b372778 __attribute__((visibility("hidden")));  // this unit's callbacks
extern UnkStrPair_0x10 g_unk_710b372748 __attribute__((visibility("hidden")));    // the second tunables file
extern base::reflection::CType g_unk_710b371c60;                                  // the registry's type

// A registered tunable: its vf_0x18 prepares it; its flag at +0x20 marks it as read.
struct UnkTunableEntry_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x18];
    u8 field_0x20;
};

struct UnkTunableObjects_b0235 {
    UnkTunableEntry_b0235** objects;
    u8 pad_0x8[8];
    u32 count;  // 0x10
};

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_710b372778, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Marks every tunable, then reads the second tunables file into the registry.
void fn_710013af08(UnkTunableObjects_b0235* self) {
    if (self->count) {
        for (UnkTunableEntry_b0235** it = self->objects; it != self->objects + self->count;) {
            (*it)->vf_0x18();
            (*it++)->field_0x20 = 1;
        }
    }
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    reader.field_0x38 = 3;
    reader.field_0x3c = 2;
    bool opened;
    {
        base::global::CRntString mode("rb");
        opened = fn_710007c1ac(&reader, &g_unk_710b372748, &mode);
    }
    if (opened)
        fn_710000ccd8(&reader);
    if (reader.field_0x38 == 0) {
        UnkReadTarget target;
        {
            base::reflection::UnkReflValue value;
            value.field_0x0 = nullptr;
            value.type = nullptr;
            value.kind = 2;
            fn_71000a18d4(&value, &g_unk_710b371c60, self, true);
            fn_71000f0568(&target, &value, 5);
            fn_71000a1c60(&value);
        }
        if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
            Parse(&reader, &target);
        for (u64 i = 0; i < target.count; ++i) {
            UnkReadEntry& e = target.entries[i];
            if (e.ops) {
                e.ops->destroy(e.storage);
                e.ops = nullptr;
            }
        }
    }
    fn_710007c0e4(&reader.file);
}

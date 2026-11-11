// Unit 92 (no reliable name; right after the reflected-object reader of src/base/UnkReflObjectRead.cpp): a listing of the
// values of a reflected object. The owner keeps a count and up to 100 entries of 0x68 bytes (the value, the typed pointer
// it resolves to, a kind, two more values and the parent entry); fn_71000ee314 starts the listing with the root value,
// fn_71000eef84 resizes it, fn_71000eed30 hands the current entry's count (an object's members or a collection's size)
// to the caller's reference. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU92.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

void fn_71000a1a24(UnkReflValue* out, const UnkTypedRef* v, u32 by_ref, u32 kind);
void fn_71000a174c(UnkTypedRef* out, const void* ref);

// One entry of the listing (0x68 bytes). The list keeps them after a 4-byte count, so they are only 4-aligned.
#pragma pack(push, 4)
struct UnkListEntry_b0236 {
    UnkReflValue value;       // 0x0
    UnkReflTypedPtr typed;    // 0x18
    u32 kind;                 // 0x28
    u32 pad_0x2c;
    UnkReflValue field_0x30;
    UnkReflValue field_0x48;
    u64 field_0x60;
};

struct UnkEntryList_b0236 {
    u32 count;
    UnkListEntry_b0236 entries[100];
};

struct UnkListOwner_b0236 {
    char* type;  // 0x0
    void* ptr;   // 0x8
    u32 field_0x10;
    UnkEntryList_b0236 list;  // 0x14
    s32 field_0x28b8;
    u8 field_0x28bc;
};
#pragma pack(pop)

void fn_71000edfc0(UnkListEntry_b0236* out, const UnkReflValue* value, void* arg);
void fn_71000ee3d8(UnkEntryList_b0236* list, const UnkListEntry_b0236* src);

static inline void DestroyEntry(UnkListEntry_b0236* e) {
    fn_71000a1c60(&e->field_0x48);
    fn_71000a1c60(&e->field_0x30);
    fn_71000a1c60(&e->value);
}

// Starts the listing with the value of the reference.
void fn_71000ee314(UnkListOwner_b0236* self, const UnkTypedRef* ref, u32 arg) {
    UnkReflValue value;
    UnkListEntry_b0236 entry;
    UnkTypedRef copy;
    char* type = const_cast<char*>(reinterpret_cast<const char*>(ref->type));
    void* ptr = ref->ptr;
    self->type = fn_710009dcd8(type, ptr);
    self->ptr = ptr;
    self->field_0x10 = arg;
    self->list.count = 0;
    self->field_0x28b8 = -1;
    self->field_0x28bc = 0;
    copy.type = ref->type;
    copy.ptr = ref->ptr;
    fn_71000a1a24(&value, &copy, 1, 2);
    fn_71000edfc0(&entry, &value, nullptr);
    fn_71000a1c60(&value);
    fn_71000ee3d8(&self->list, &entry);
    DestroyEntry(&entry);
}

// Grows the listing with empty entries or destroys the ones past the new count.
void fn_71000eef84(UnkEntryList_b0236* list, u32 n) {
    u32 count = list->count;
    if (count < n) {
        UnkListEntry_b0236* e = &list->entries[count];
        for (u32 i = count; i < n; ++i, ++e) {
            UnkReflValue value;
            UnkTypedRef none = {nullptr, nullptr};
            fn_71000a1a24(&value, &none, 1, 2);
            fn_71000edfc0(e, &value, nullptr);
            fn_71000a1c60(&value);
        }
    } else if (count > n) {
        for (u64 i = n; i < list->count; ++i)
            DestroyEntry(&list->entries[i]);
    }
    list->count = n;
}

// Hands the current entry's count to `out`: the number of members of an object (kind 0) or the size of a collection
// (kind 1, through its type's slot 0x108), or the entry's own typed reference (kind 2).
bool fn_71000eed30(u32 flag, UnkListEntry_b0236* entry, UnkTypedRef* out) {
    UnkTypedRef src;
    u32 size_count;
    UnkTypedRef tmp;
    UnkTypedRef ref2;
    const CType* type;
    switch (entry->kind) {
    case 0: {
        UnkReflObject* object = reinterpret_cast<UnkReflObject*>(entry->typed.type);
        if (!object)
            return true;
        if (!fn_710009d50c(object->vf_0x0(), fn_7100096100()))
            return true;
        u32 count = *reinterpret_cast<u32*>(reinterpret_cast<char*>(object) + 0xf8);
        type = out->type;
        src.type = fn_7100005178();
        src.ptr = &count;
        fn_710009d7a8(type, out, &src);
        return true;
    }
    case 1: {
        UnkReflObject* object = reinterpret_cast<UnkReflObject*>(entry->typed.type);
        if (!object)
            return true;
        if (!fn_710009d50c(object->vf_0x0(), fn_71000986d0()))
            return true;
        char* t = entry->typed.type;
        void* p = entry->typed.ptr;
        ref2.type = reinterpret_cast<const CType*>(fn_710009dcd8(t, p));
        ref2.ptr = p;
        fn_71000a174c(&tmp, &ref2);
        u32 (*size)(void*) = *reinterpret_cast<u32 (**)(void*)>(reinterpret_cast<char*>(object) + 0x108);
        if (size && tmp.type == reinterpret_cast<const CType*>(object))
            size_count = size(tmp.ptr);
        else
            size_count = 0;
        type = out->type;
        src.type = fn_7100005178();
        src.ptr = &size_count;
        fn_710009d7a8(type, out, &src);
        return true;
    }
    case 2: {
        char* t = entry->typed.type;
        void* p = entry->typed.ptr;
        UnkTypedRef ref;
        ref.type = reinterpret_cast<const CType*>(fn_710009dcd8(t, p));
        ref.ptr = p;
        type = out->type;
        fn_71000a174c(&src, &ref);
        fn_710009d7a8(type, out, &src);
        return true;
    }
    }
    return true;
}

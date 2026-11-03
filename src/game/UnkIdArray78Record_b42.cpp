// Unit 2640 (tools/tu_map.py, no name hint): the constructor, copy constructor and copy assignment of a reflected
// record holding a string id and an array (base::UnkVector's layout) of 0x78-byte elements, through
// the unit's out-of-line Reserve (fn_710100147c), push-back (fn_7101001694) and clear
// (fn_71010017f4), the ones fn_71010012d4 (src/game/GUI/CMissionLog/fn_7100b5bfdc.cpp) copies with.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The element and array types of src/game/GUI/CMissionLog/fn_7100b5bfdc.cpp (same names, same
// layouts), so the callees keep that file's signatures.
struct UnkElem78 {
    u8 unk_0x0[0x78];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);
void fn_7101001694(UnkElemArray<UnkElem78>* self, const UnkElem78* value);
void fn_71010017f4(UnkElemArray<UnkElem78>* self);  // clear

struct UnkIdArray78Record_b42a3 {
    CStrId id;
    UnkElemArray<UnkElem78> items;
};

static inline void AppendAll_b42a3(UnkElemArray<UnkElem78>* self, const UnkElemArray<UnkElem78>& other) {
    fn_710100147c(self, other.capacity);
    for (const UnkElem78 *it = other.data, *end = it + other.count; it != end; ++it)
        fn_7101001694(self, it);
}

// The record's value-initialising constructor (cleared first): the empty id (the pool created first
// when there is none) and an empty array (allocator flags 0xf).
void fn_71010027ac(UnkIdArray78Record_b42a3* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->items.data = nullptr;
    self->items.count = 0;
    self->items.capacity = 0;
    self->items.step = 10;
    self->items.field_0x14 = true;
    self->items.flags = 0xf;
    self->items.allocator = fn_7100083990();
}

// The array's copy assignment on its own.
UnkElemArray<UnkElem78>* fn_7101001398(UnkElemArray<UnkElem78>* self, const UnkElemArray<UnkElem78>* other) {
    if (self != other) {
        fn_71010017f4(self);
        AppendAll_b42a3(self, *other);
    }
    return self;
}

// Signatures as src/reflection/class_descriptors_02.cpp declares them.
void fn_7101002888(void* dst, const void* src) {
    UnkIdArray78Record_b42a3* self = static_cast<UnkIdArray78Record_b42a3*>(dst);
    const UnkIdArray78Record_b42a3& other = *static_cast<const UnkIdArray78Record_b42a3*>(src);
    new (&self->id) CStrId(other.id);
    self->items.data = nullptr;
    self->items.count = 0;
    self->items.capacity = 0;
    self->items.step = other.items.step;
    self->items.field_0x14 = other.items.field_0x14;
    self->items.flags = other.items.flags;
    self->items.allocator = other.items.allocator;
    AppendAll_b42a3(&self->items, other.items);
}

void* fn_7101002a08(void* dst, const void* src) {
    UnkIdArray78Record_b42a3* self = static_cast<UnkIdArray78Record_b42a3*>(dst);
    const UnkIdArray78Record_b42a3& other = *static_cast<const UnkIdArray78Record_b42a3*>(src);
    if (self != &other) {
        self->id = other.id;
        fn_71010017f4(&self->items);
        AppendAll_b42a3(&self->items, other.items);
    }
    return self;
}

// A hash map of these records keyed by string id (bucket = the two hash words of the key's string
// xored, modulo the bucket count; chained through +0x48): the setter of the reflected collection
// type (slot 0x140), which assigns the given record to the key's node when there is one.
struct UnkIdArray78MapNode_b42a3 {
    CStrId key;
    UnkIdArray78Record_b42a3 value;
    u8 unk_0x38[0x10];
    UnkIdArray78MapNode_b42a3* chain;
};

struct UnkIdArray78Map_b42a3 {
    u8 unk_0x0[0x10];
    UnkIdArray78MapNode_b42a3** buckets;
    u32 count;
    u32 bucket_count;
};

static inline UnkIdArray78MapNode_b42a3* Find_b42a3(UnkIdArray78Map_b42a3* self, const CStrId* key) {
    UnkIdArray78MapNode_b42a3** slot = nullptr;
    if (self->count) {
        const char* raw = reinterpret_cast<const char*>(key->data);
        u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
        u32 i = h % self->bucket_count;
        slot = self->buckets + i;
    }
    if (!slot)
        return nullptr;
    UnkIdArray78MapNode_b42a3* n = *slot;
    if (!n)
        return nullptr;
    while (n->key.data != key->data) {
        n = n->chain;
        if (!n)
            return nullptr;
    }
    return n;
}

void fn_710100363c(UnkIdArray78Map_b42a3* self, const CStrId* key, const UnkIdArray78Record_b42a3* value) {
    UnkIdArray78MapNode_b42a3* n = Find_b42a3(self, key);
    if (!n)
        return;
    UnkIdArray78Record_b42a3* dst = &n->value;
    dst->id = value->id;
    if (dst != value) {
        fn_71010017f4(&dst->items);
        AppendAll_b42a3(&dst->items, value->items);
    }
}

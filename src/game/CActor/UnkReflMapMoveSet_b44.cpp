// A reflected hash map's "set by key" slot (CCollectionType +0x148) for a map whose key is an object
// pointer (hashed from its two words at +0x30) and whose value is a growable array of refcounted
// pointers: the entry is found by key; when found, its array's objects are released (count dropped,
// the object's slot 2 called at zero) and the given array is moved in. Names and layouts are
// placeholders.
#include "types.h"


struct UnkRefObj_b44a1 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    s32 ref_count;
};

struct UnkHashKeyObj_b44a1 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkRefArray_b44a1 {
    UnkRefObj_b44a1** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    void* allocator;
};

struct UnkMapNode_b44a1 {
    UnkHashKeyObj_b44a1* key;
    UnkRefArray_b44a1 value;
    u8 pad_0x30[0x10];
    UnkMapNode_b44a1* next;
};

struct UnkMap_b44a1 {
    u8 pad_0x0[0x10];
    UnkMapNode_b44a1** buckets;
    u32 count;
    u32 bucket_count;
};

void fn_7100244f84(UnkMap_b44a1* self, UnkHashKeyObj_b44a1* const* key, UnkRefArray_b44a1* value) {
    if (self->count == 0)
        return;
    UnkHashKeyObj_b44a1* k = *key;
    u32 idx = (k->hash_lo ^ k->hash_hi) % self->bucket_count;
    UnkMapNode_b44a1** bucket = &self->buckets[idx];
    if (!bucket)
        return;
    UnkMapNode_b44a1* node = *bucket;
    for (; node; node = node->next)
        if (node->key == k)
            break;
    if (!node)
        return;
    UnkRefArray_b44a1* dst = &node->value;
    if (dst == value)
        return;
    for (u32 i = 0; i < dst->count; ++i) {
        if (UnkRefObj_b44a1* p = dst->data[i]) {
            if (--*reinterpret_cast<s32*>(reinterpret_cast<char*>(p) + 8) == 0)
                p->Release();
        }
    }
    dst->count = 0;
    dst->allocator = value->allocator;
    dst->flags = value->flags;
    dst->step = value->step;
    dst->growable = value->growable;
    dst->count = value->count;
    dst->capacity = value->capacity;
    dst->data = value->data;
    value->data = nullptr;
    value->count = 0;
    value->capacity = 0;
}

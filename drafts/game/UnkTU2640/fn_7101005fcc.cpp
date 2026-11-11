#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkHashView_b0213 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkMapValue_b0213 {
    base::global::UnkStringData* id;
    void* ptr;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 field_0x1c;
    u32 field_0x20;
    u64 field_0x28;

    UnkMapValue_b0213(UnkMapValue_b0213&& o)
        : id(o.id) {
        void* p = o.ptr;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&o.id, &empty, sizeof(empty));
        ptr = p;
        // The remaining fields are copied bytewise, so they are read after the id is replaced.
        __builtin_memcpy(&field_0x10, &o.field_0x10, 4);
        __builtin_memcpy(&field_0x14, &o.field_0x14, 4);
        __builtin_memcpy(&field_0x18, &o.field_0x18, 4);
        __builtin_memcpy(&field_0x1c, &o.field_0x1c, 1);
        __builtin_memcpy(&field_0x20, &o.field_0x20, 4);
        __builtin_memcpy(&field_0x28, &o.field_0x28, 8);
        __builtin_memset(&o.ptr, 0, 16);
    }
};

struct UnkMapNode_b0213 {
    CStrId key;
    UnkMapValue_b0213 value;
    UnkMapNode_b0213* next;
    UnkMapNode_b0213* prev;
    UnkMapNode_b0213* chain;
};

struct UnkMap_b0213 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkMapNode_b0213** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x20[0x18];
    UnkMapNode_b0213* tail;
    UnkMapNode_b0213* head;
};

void fn_7101005fcc(UnkMap_b0213* self, const CStrId* key, UnkMapValue_b0213* value) {
    UnkMapNode_b0213** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkHashView_b0213*>(key->data)->hash;
        u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
        asm("" : "+r"(i));  // the bucket index is computed before the bucket array is loaded
        slot = &self->buckets[i];
    }
    UnkMapNode_b0213* chain = *slot;
    auto* n = static_cast<UnkMapNode_b0213*>(self->allocator->vf_0x40(0x50, 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        new (&n->value) UnkMapValue_b0213(static_cast<UnkMapValue_b0213&&>(*value));
        n->chain = nullptr;
        n->prev = nullptr;
        n->next = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        UnkMapNode_b0213* tail = self->tail;
        __builtin_memcpy(&n->prev, &tail, sizeof(tail));
        self->tail->next = n;
    }
    self->tail = n;
    ++self->count;
}

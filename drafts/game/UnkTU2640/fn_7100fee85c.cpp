#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkHashView_b0213 {
    u8 pad_0x0[0x30];
    u64 hash;
};

void fn_7100fed7b0(void* dst, void* src);

struct UnkIdMapValue_b0213 {
    u8 unk_0x0[0x50];
};

struct UnkIdMapNode_b0213 {
    CStrId key;
    UnkIdMapValue_b0213 value;
    UnkIdMapNode_b0213* next;  // 0x58
    UnkIdMapNode_b0213* prev;
    UnkIdMapNode_b0213* chain;
};

struct UnkIdMap_b0213 {
    CStrId id;
    base::UnkAllocator* allocator;  // 0x8
    u32 flags;
    u32 count;
    UnkIdMapNode_b0213** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x28[0x18];
    UnkIdMapNode_b0213* tail;  // 0x40
    UnkIdMapNode_b0213* head;  // 0x48
};

UnkIdMap_b0213* fn_7100fee85c(UnkIdMap_b0213* self, const UnkIdMap_b0213* other) {
    if (self == other)
        return self;
    self->id = other->id;
    for (const UnkIdMapNode_b0213* it = other->head; it; it = it->next) {
        UnkIdMapNode_b0213** slot = nullptr;
        if (self->bucket_count) {
            u64 h = reinterpret_cast<const UnkHashView_b0213*>(it->key.data)->hash;
            u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
            asm("" : "+r"(i));  // the bucket index is computed before the bucket array is loaded
            slot = &self->buckets[i];
        }
        UnkIdMapNode_b0213* chain = *slot;
        auto* n = static_cast<UnkIdMapNode_b0213*>(self->allocator->vf_0x40(0x70, 8, self->flags));
        if (n) {
            void* src = const_cast<UnkIdMapValue_b0213*>(&it->value);
            new (&n->key) CStrId(it->key);
            fn_7100fed7b0(&n->value, src);
            n->chain = nullptr;
            n->prev = nullptr;
            n->next = nullptr;
        }
        n->chain = chain;
        *slot = n;
        if (!self->head) {
            self->head = n;
        } else if (self->tail) {
            UnkIdMapNode_b0213* tail = self->tail;
            __builtin_memcpy(&n->prev, &tail, sizeof(tail));
            self->tail->next = n;
        }
        self->tail = n;
        ++self->count;
    }
    return self;
}

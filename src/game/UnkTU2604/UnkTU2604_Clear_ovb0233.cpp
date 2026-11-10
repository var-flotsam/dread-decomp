// Unit 2604: clearing a string-id map (fn_7100f8db84). Names and layouts are placeholders.
#include <string.h>
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkMapId_b0233 {
    UnkStringData* data;

    void Copy(const UnkMapId_b0233& other) {
        data = other.data;
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    void Take(UnkMapId_b0233& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMapNode_b0233 {
    UnkMapId_b0233 key;    // 0x0
    UnkMapId_b0233 value;  // 0x8
    u64 words[2];          // 0x10
    u32 count;             // 0x20
    u8 pad_0x24[4];
    UnkMapNode_b0233* next;   // 0x28
    UnkMapNode_b0233* prev;   // 0x30
    UnkMapNode_b0233* chain;  // 0x38
};

struct UnkIdMap_b0233 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 size;                       // 0xc
    UnkMapNode_b0233** buckets;     // 0x10
    u32 has_buckets;                // 0x18
    u32 bucket_count;               // 0x1c
    u8 pad_0x20[0x18];
    UnkMapNode_b0233* tail;         // 0x38
    UnkMapNode_b0233* head;         // 0x40
};


// Frees every entry and empties the buckets.
void fn_7100f8db84(UnkIdMap_b0233* self) {
    if (!self->size)
        return;
    UnkMapNode_b0233* node = self->head;
    while (node) {
        UnkMapNode_b0233* next = node->next;
        node->value.Release();
        node->key.Release();
        self->allocator->vf_0x48(node);
        node = next;
    }
    memset(self->buckets, 0, static_cast<u64>(self->bucket_count) << 3);
    self->size = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

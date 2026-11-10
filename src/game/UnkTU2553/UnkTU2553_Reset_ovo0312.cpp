// Unit 2553 of tools/tu_map.py (no reliable name; hint CGameManager::CTunableGameManager): resetting a
// string-id keyed map and its block pool. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

// ---- fn_7100de7c70: the map's nodes released and freed, its buckets cleared, then the block pool reset.
struct UnkIdMapNode_o0312 {
    CStrId key;
    u8 pad_0x8[0x8];
    UnkIdMapNode_o0312* next;  // 0x10
};

struct UnkIdMapOwner_o0312 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    void** buckets;                 // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkIdMapNode_o0312* tail;  // 0x38
    UnkIdMapNode_o0312* head;  // 0x40
    void* pool_vtable;         // 0x48
    base::UnkAllocator* pool_allocator;  // 0x50
    u8 pool_list[0x20];                  // 0x58
    void* pool_block;                    // 0x78
    u8 pad_0x80[4];
    u8 field_0x84;
};
static_assert(__builtin_offsetof(UnkIdMapOwner_o0312, field_0x84) == 0x84, "layout");

void fn_7100e05f58(char* self);

void fn_7100de7c70(char* self_) {
    UnkIdMapOwner_o0312* self = reinterpret_cast<UnkIdMapOwner_o0312*>(self_);
    self->field_0x84 = 1;
    if (self->count) {
        UnkIdMapNode_o0312* node = self->head;
        while (node) {
            UnkIdMapNode_o0312* next = node->next;
            node->key.Reset();
            self->allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(self->buckets, 0, self->bucket_capacity * u64(8));
        self->count = 0;
        self->tail = nullptr;
        self->head = nullptr;
    }
    self->allocator = nullptr;
    void* block = self->pool_block;
    self->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    if (block) {
        fn_71000837f4(reinterpret_cast<UnkBlockFreeList*>(self->pool_list));
        self->pool_allocator->vf_0x48(block);
        self->pool_allocator = nullptr;
    }
    fn_7100e05f58(self_);
}

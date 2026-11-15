// malloc unit: sets up a string-keyed map with its own block pool, then empties it (the constructor
// of the original with the clear inlined). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/malloc.h"
#include "types.h"

struct UnkPoolMapNode {
    base::UnkStrIdMember key;
    u8 pad_0x8[8];
    UnkPoolMapNode* next;
};

struct UnkPoolMapBlock {
    u8 pad_0x0[8];
    UnkPoolMapBlock* next;
};

struct UnkStatsObject {
    base::UnkAllocator* allocator;
    u8 pad_0x8[4];
    u32 count;
    void** buckets;
    u8 pad_0x18[4];
    u32 bucket_count;
    u8 pad_0x20[0x18];
    UnkPoolMapNode* last;
    UnkPoolMapNode* first;
    char* pool_vtable;
    u32 pool_field_0x50;
    UnkPoolMapBlock* pool_blocks;
    void* pool_field_0x60;
    base::UnkAllocator* pool_allocator;
    u32 pool_field_0x70;
    u8 pad_0x74[4];
};

void fn_710013208c(UnkStatsObject* self, u32 n, base::UnkAllocator* allocator, u32 elem);

void fn_7100131f48(UnkStatsObject* self) {
    fn_710013208c(self, 0x12c, nullptr, 0x18);
    self->pool_blocks = nullptr;
    self->pool_vtable = reinterpret_cast<char*>(g_unk_7101a1bdf8) + 0x10;
    self->pool_field_0x50 = 0;
    self->pool_field_0x60 = nullptr;
    self->pool_allocator = fn_7100083990();
    self->pool_field_0x70 = 0x13;
    if (self->count) {
        UnkPoolMapNode* node = self->first;
        while (node) {
            UnkPoolMapNode* next = node->next;
            node->key.~UnkStrIdMemberOf();
            self->allocator->vf_0x48(node);
            node = next;
        }
        memset(self->buckets, 0, self->bucket_count * u64(8));
        self->count = 0;
        self->last = nullptr;
        self->first = nullptr;
    }
    for (UnkPoolMapBlock* block = self->pool_blocks; block;) {
        UnkPoolMapBlock* next = block->next;
        self->pool_allocator->vf_0x48(block);
        block = next;
    }
    self->pool_field_0x50 = 0;
    self->pool_blocks = nullptr;
    self->pool_field_0x60 = nullptr;
}

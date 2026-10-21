// The string heap's size-class setup: allocates the class's memory, block headers and free
// stack, then gives every block its slice of the memory (zeroed) and pushes it as free.
#include <string.h>

#include "base/global/UnkStringHeap.h"

using base::global::UnkStringBlock;
using base::global::UnkStringBlockClass;

// The byte count is computed before the allocator is fetched, as in the original.
void fn_7100083c48(UnkStringBlockClass* self, u32 count, u32 block_size, u32 index) {
    u32 total = block_size * count;
    self->free_count = count;
    self->block_size = block_size;
    self->count = count;
    self->min_free = count;
    self->memory = static_cast<char*>(fn_7100083990()->vf_0x40(total, 8, 1));
    self->headers = static_cast<UnkStringBlock*>(
        fn_7100083990()->vf_0x40((u64)self->count * sizeof(UnkStringBlock), 8, 1));
    self->blocks = static_cast<UnkStringBlock**>(
        fn_7100083990()->vf_0x40((u64)self->count * sizeof(UnkStringBlock*), 8, 1));
    for (u32 i = 0; i < self->count; ++i) {
        UnkStringBlock* block = &self->headers[i];
        block->memory = self->memory + self->block_size * i;
        memset(block->memory, 0, self->block_size);
        block->index = i;
        block->size_class = index;
        block->used = false;
        self->blocks[i] = block;
    }
}

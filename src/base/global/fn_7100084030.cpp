// The string heap's allocate: takes a free block from the first size class whose blocks are
// larger than the request, and warns when fewer than 10 are left.
#include <stdio.h>

#include "base/global/UnkStringHeap.h"

using base::global::UnkStringBlock;
using base::global::UnkStringBlockClass;
using base::global::UnkStringHeap;

UnkStringBlock* fn_7100084030(UnkStringHeap* heap, u32 size) {
    UnkStringBlockClass* c = nullptr;
    // The loop tests a flag set from the selected pointer, as the original does.
    bool none = true;
    for (u32 i = 0; none && i < 6; ++i) {
        c = heap->classes[i].block_size > size ? &heap->classes[i] : nullptr;
        none = c == nullptr;
    }
    if (none)
        return nullptr;
    u32 n = --c->free_count;
    if (n < c->min_free)
        c->min_free = n;
    if (n < 10) {
        printf("warning! string pool (%u) with less than 10 free elements!!!", c->block_size);
        n = c->free_count;
    }
    UnkStringBlock* block = c->blocks[n];
    block->used = true;
    return block;
}

// The device layer's heaps (tools/tu_map.py unit 170, 0x710015334c-0x7100153c20): a heap allocates
// from its dlmalloc space at +0x120. Holds the reallocation fn_7100153a10 (src/engine/UnkGfxSamplerSlots.cpp
// grows the descriptor storage with it). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
    u8 pad_0x108[0x18];
    void* space;  // 0x120
};

// Allocates `size` bytes (at least 0x10) at an alignment, copying the old block's `size` bytes when
// there is one; the old block is not freed.
void* fn_7100153a10(UnkGfxHeap* heap, void* old, u64 size, u64 align) {
    u64 n = size ? size : 0x10;
    void* p = align == 8 ? fn_7100081860(heap->space, n) : fn_71000829f0(heap->space, align, n);
    if (old)
        memcpy(p, old, size);
    return p;
}

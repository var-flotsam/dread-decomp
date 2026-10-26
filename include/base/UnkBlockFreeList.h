#pragma once

// The free list of a block pool: an allocator that hands out `elem`-byte blocks of one buffer of
// `size` bytes (the embedded sub-object with vtable g_unk_7101a1a4c8 that pooled lists and hash
// maps keep next to their allocator; base/UnkCloneHelpers.h has the pool object around it). Its
// constructor, reset and init over a buffer are fn_710008377c, fn_71000837f4 and fn_710008378c,
// in the default allocator's unit. Names and layouts are placeholders inferred from the code.

#include "types.h"

struct UnkBlockFreeList {
    u64 size;
    u64 elem;
    u64 field_0x10;
    u64 count;
    void* buffer;
};

void fn_710008377c(UnkBlockFreeList* list);                                     // constructor
void fn_71000837f4(UnkBlockFreeList* list);                                     // reset (the buffer stays)
void fn_710008378c(UnkBlockFreeList* list, void* buffer, u64 size, u64 elem);  // init over a buffer
extern char g_unk_7101a1a4c8[];  // the block pool's vtable (the pool object: base/UnkCloneHelpers.h)

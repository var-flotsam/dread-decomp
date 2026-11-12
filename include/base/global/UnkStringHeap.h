#pragma once

// The string pool's heap (g_unk_7101d58d40): six size classes of 16 to 512 bytes, each a fixed
// number of blocks carved from one allocation of the default allocator, with a stack of the
// free ones. fn_7100084030 allocates, fn_7100084144 frees. Names and layouts are placeholders inferred from the code.

#include "base/UnkVector.h"
#include "types.h"

namespace base::global {

// A block's header: its memory, its index in the class, the class, and whether it is in use.
struct UnkStringBlock {
    char* memory;
    u32 index;
    u8 size_class;
    bool used;
};

struct UnkStringBlockClass {
    char* memory;            // count * block_size bytes
    UnkStringBlock* headers;  // count of them
    UnkStringBlock** blocks;  // a stack; the free ones first
    u32 free_count;
    // A union member: Clang gives union accesses no type-based alias info, so the class setup
    // reloads it after storing each block's memory pointer, as the original does.
    union {
        u32 block_size;
    };
    u32 count;
    u32 min_free;  // the fewest free blocks so far

    // Set up by fn_7100083c48 (from fn_7100083f74, with 16 << class bytes per block).
    // Inlined in the heap's clear (fn_7100083d90).
    void Free() {
        fn_7100083990()->vf_0x48(memory);
        memory = nullptr;
        fn_7100083990()->vf_0x48(headers);
        headers = nullptr;
        fn_7100083990()->vf_0x48(blocks);
        blocks = nullptr;
        free_count = 0;
        block_size = 0;
        count = 0;
    }
};

struct UnkStringHeap {
    UnkStringHeap() asm("fn_7100083d48");   // fn_7100083d48
    ~UnkStringHeap() asm("fn_7100083d8c");  // fn_7100083d8c

    UnkStringBlockClass classes[6];
    bool initialized;  // +0xf0
};

}  // namespace base::global

// Sets up each size class with counts[class] blocks.
void fn_7100083f74(base::global::UnkStringHeap* heap, const u32* counts);

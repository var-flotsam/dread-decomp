// The base::snd::CSoundSystemATK unit (tools/tu_map.py unit 99): the sound engine heap's allocation
// (the heap's layout is UnkAtkHeap in src/base/snd/CSoundSystemATK.cpp): allocates from the
// sub-allocator at +0x8, keeps the result at +0x48, runs the hook (+0x50, argument +0x58) and
// returns the kept result, which the hook may have replaced. The second entry is the same call
// reached through the sub-allocator (this - 8). Names are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU58.h"
#include "types.h"

struct UnkAtkHeap99 {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 heap[8];                       // 0x40
    void* last;                       // 0x48
    void (*hook)(void* arg, UnkAtkHeap99* heap);
    void* hook_arg;
};

static inline void* Allocate99(UnkAtkHeap99* heap, base::UnkSubAllocator* allocator, u64 size, u64 align,
                               u32 flags) {
    heap->last = fn_71000813e4(allocator, size, align, flags);
    if (heap->hook) {
        heap->hook(heap->hook_arg, heap);
    }
    return heap->last;
}

void* fn_71000f8680(UnkAtkHeap99* self, u64 size, u64 align, u32 flags) {
    return Allocate99(self, &self->allocator, size, align, flags);
}

void* fn_7100103974(base::UnkSubAllocator* self, u64 size, u64 align, u32 flags) {
    UnkAtkHeap99* heap = reinterpret_cast<UnkAtkHeap99*>(reinterpret_cast<u8*>(self) - 8);
    return Allocate99(heap, self, size, align, flags);
}

#pragma once

// A 0x48-byte object: an allocator, a word, and an array of 11 slots, null at first. The render pass
// list's unit creates one on first use (fn_71001fc6d4 keeps it in g_unk_716e598120; its constructor
// is fn_710021812c, src/game/UnkRenderPassSlots.cpp), and units 1674 and 2734 of tools/tu_map.py
// construct the same type out of line (fn_7100b1df48, fn_710119d484). Names and layouts are
// placeholders.

#include "base/UnkVector.h"
#include "types.h"

struct UnkRenderPassSlots {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 field_0xc;
    base::UnkVector<void*> slots;
    u64 field_0x38;
    u64 field_0x40;

    __attribute__((always_inline)) UnkRenderPassSlots();
};
static_assert(sizeof(UnkRenderPassSlots) == 0x48, "UnkRenderPassSlots layout");

// Sets the array to `n` elements: new ones are null; the array only grows to exactly `n`.
__attribute__((always_inline)) inline void UnkResizeSlots(base::UnkVector<void*>& v, u32 n) {
    u32 count = v.count;
    if (count < n) {
        v.Reserve(n);
        __builtin_memset(&v.data[v.count], 0, (u64)(n - v.count) * sizeof(void*));
    } else if (count == n) {
        return;
    }
    v.count = n;
}

inline UnkRenderPassSlots::UnkRenderPassSlots()
    : allocator(fn_7100083990()), field_0x8(0xe), field_0xc(0), slots(0xe), field_0x38(0), field_0x40(0) {
    UnkResizeSlots(slots, 11);
}

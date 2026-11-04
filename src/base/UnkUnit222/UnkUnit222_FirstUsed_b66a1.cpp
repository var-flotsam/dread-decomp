// tools/tu_map.py unit 222 (no class hint): the index of the first 0x98-byte slot whose pointer at
// 0x60 is set, or -1. Names and layouts are placeholders.
#include "types.h"

struct UnkSlot222_b66a1 {
    u8 pad_0x0[0x60];
    void* used;  // 0x60
    u8 pad_0x68[0x30];
};

struct UnkSlotArray222_b66a1 {
    u8 pad_0x0[0x8];
    UnkSlot222_b66a1* slots;  // 0x8
    u8 pad_0x10[0x8];
    u32 count;  // 0x18
};

static inline UnkSlot222_b66a1* FirstUsed(UnkSlotArray222_b66a1* self) {
    for (UnkSlot222_b66a1* it = self->slots, *end = it + self->count; it != end; ++it) {
        if (it->used != nullptr) {
            return it;
        }
    }
    return nullptr;
}

s32 fn_710019baa8(UnkSlotArray222_b66a1* self) {
    UnkSlot222_b66a1* slot = FirstUsed(self);
    if (slot == nullptr) {
        return -1;
    }
    return static_cast<s32>(slot - self->slots);
}

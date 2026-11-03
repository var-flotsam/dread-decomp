// CLogicDroppedItemManager::CTunableDroppableBillboard unit (tools/tu_map.py): sums the byte flag at +0x7d over
// the 128 slots of 0xf0 bytes, and flags (+0x9c) the active slots whose value at +0x54 exceeds a limit. Names
// and layouts are placeholders.
#include "types.h"

struct UnkDroppedSlot_b47a3 {
    u8 unk_0x0[0x7d];
    u8 active;
    u8 unk_0x7e[0xf0 - 0x7e];
};

struct UnkDroppedSlots_b47a3 {
    UnkDroppedSlot_b47a3 slots[128];
};

u32 fn_7100e2d608(UnkDroppedSlots_b47a3* self) {
    u32 n = 0;
    for (u64 i = 0; i < 128; ++i)
        n += self->slots[i].active;
    return n;
}

struct UnkDroppedSlotFlags_b47a3 {
    u8 unk_0x0[0x54];
    f32 value;
    u8 unk_0x58[0x7d - 0x58];
    u8 active;
    u8 unk_0x7e[0x9c - 0x7e];
    u8 flag;
    u8 unk_0x9d[0xf0 - 0x9d];
};

struct UnkDroppedSlotsFlags_b47a3 {
    UnkDroppedSlotFlags_b47a3 slots[128];
};

void fn_7100e2d638(UnkDroppedSlotsFlags_b47a3* self, f32 limit) {
    for (s32 i = 0; i < 128; ++i) {
        if (self->slots[i].active && self->slots[i].value > limit)
            self->slots[i].flag = 1;
    }
}

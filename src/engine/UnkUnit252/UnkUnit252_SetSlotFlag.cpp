// tools/tu_map.py unit 252 (no class hint): for a slot below 5 and below the array's count (+0xf8),
// sets the flag +0x8 of the array element (+0xf0) that fn_71001c04bc returns. Names are
// placeholders.
#include "types.h"

struct UnkSlot252 {
    u8 pad_0x0[0x8];
    bool flag;  // 0x8
};

struct UnkSlotArray252 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
};

UnkSlot252* fn_71001c04bc(UnkSlotArray252* array, u32 index);

struct UnkSlotOwner252 {
    u8 pad_0x0[0xf0];
    UnkSlotArray252 slots;  // 0xf0
};

void fn_71001be430(UnkSlotOwner252* self, u32 index, bool flag) {
    if (index <= 4 && index < self->slots.count) {
        fn_71001c04bc(&self->slots, index)->flag = flag;
    }
}

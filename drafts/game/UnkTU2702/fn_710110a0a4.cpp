#include "types.h"

struct UnkSlotSet_b0143 {
    void** vtable;
    u32 vf_0x28() { return reinterpret_cast<u32 (*)(UnkSlotSet_b0143*)>(vtable[0x28 / 8])(this); }
};

struct UnkSlot_b0143 {
    u8 pad_0x0[0x1c];
    s16 field_0x1c;
    u8 pad_0x1e[0x1a];
    s16 field_0x38;
    u8 pad_0x3a[0xe6];
};
static_assert(sizeof(UnkSlot_b0143) == 0x120, "UnkSlot_b0143 layout");

struct UnkSlotSelf_b0143 {
    u8 pad_0x0[0x19c0];
    u8 field_0x19c0;
    u8 pad_0x19c1[0xf];
    u8 field_0x19d0;
    u8 pad_0x19d1[7];
    UnkSlotSet_b0143 set;  // 0x19d8
};

bool fn_710110a0a4(UnkSlotSelf_b0143* self) {
    if (!self->field_0x19c0 || !self->field_0x19d0)
        return false;
    UnkSlotSet_b0143* set = &self->set;
    if (set->vf_0x28() == 0)
        return false;
    UnkSlot_b0143* slot = reinterpret_cast<UnkSlot_b0143*>(reinterpret_cast<u8*>(self) + 0x19e2);
    u64 i = 0;
    do {
        if (slot->field_0x1c == -1)
            goto next;
        if (slot->field_0x38 != -1)
            return true;
    next:
        ++i;
        ++slot;
    } while (i < set->vf_0x28());
    return false;
}

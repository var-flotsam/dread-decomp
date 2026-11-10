// Unit 215 (tools/tu_map.py): a table of 0x84-byte slots with a list of the active ones: a slot's
// parameters are copied in and the slot switched on (fn_710019684c). Names and layouts are placeholders.
#include "types.h"

struct UnkSlot_b0238 {
    u32 words[23];       // 0x0
    u8 unk_0x5c[0x20];
    u32 link;            // 0x7c: index in the active list, -1 when inactive
    u8 active;           // 0x80
    u8 field_0x81;
    u8 field_0x82;
    u8 pad_0x83;
};
static_assert(sizeof(UnkSlot_b0238) == 0x84, "layout");

struct UnkSlotTable_b0238 {
    u8 unk_0x0[0x120];
    UnkSlot_b0238* slots;    // 0x120
    UnkSlot_b0238** active;  // 0x128
    u32 active_count;        // 0x130
    u8 unk_0x134;
    u8 dirty_a;              // 0x135
    u8 unk_0x136;
    u8 dirty_b;              // 0x137
};

void fn_710019684c(UnkSlotTable_b0238* self, u32 index, const UnkSlot_b0238* params) {
    UnkSlot_b0238* slot = &self->slots[index];
    if (slot != params) {
        slot->words[0] = params->words[0];
        for (u32 i = 1; i < 23; ++i)
            self->slots[index].words[i] = params->words[i];
    }
    self->slots[index].active = 1;
    self->dirty_a = 1;
}

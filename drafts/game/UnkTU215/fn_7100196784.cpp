// Unit 215 (tools/tu_map.py): switches a slot of the table on (appending it to the active list) or off
// (moving the list's last entry into its place). Names and layouts are placeholders.
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

void fn_7100196784(UnkSlotTable_b0238* self, u32 index, bool enable) {
    u32& count = self->active_count;
    u32 link = self->slots[index].link;
    if (enable) {
        if (link != 0xffffffffu)
            return;
        self->active[count] = &self->slots[index];
        UnkSlot_b0238& slot = self->slots[index];
        slot.link = count;
        slot.active = 1;
        slot.field_0x81 = 1;
        slot.field_0x82 = 1;
        count++;
    } else {
        if (link == 0xffffffffu || link >= count)
            return;
        u32 last = --count;
        if (link < last) {
            UnkSlot_b0238* moved = self->active[last];
            self->active[link] = moved;
            moved->link = link;
        }
        self->slots[index].link = 0xffffffffu;
    }
    self->dirty_a = 1;
    self->dirty_b = 1;
}

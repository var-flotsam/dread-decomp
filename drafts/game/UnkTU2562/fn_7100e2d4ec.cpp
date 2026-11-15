// Dropped-item manager's unit (2562): the box around the active slots' positions, each widened by 100
// on x and y; whether there was any. Names and layouts are placeholders.
#include "types.h"

struct UnkSlot_le2d4ec {
    u8 pad_0x0[0x75];
    u8 active;  // 0x75
    u8 pad_0x76[6];
    f32 x, y, z;  // 0x7c
    u8 pad_0x88[0xf0 - 0x88];
};
static_assert(sizeof(UnkSlot_le2d4ec) == 0xf0, "size");

struct UnkSlotManager_le2d4ec {
    u8 pad_0x0[8];
    UnkSlot_le2d4ec slots[128];  // 0x8
};

struct UnkBox_le2d4ec {
    f32 min_x, min_y, min_z;
    f32 max_x, max_y, max_z;

    void Include(f32 x, f32 y, f32 z) {
        f32 lx = min_x, ly = min_y, lz = min_z;
        f32 hx = max_x, hy = max_y, hz = max_z;
        min_x = lx < x ? lx : x;
        min_y = ly < y ? ly : y;
        min_z = lz < z ? lz : z;
        max_x = hx > x ? hx : x;
        max_y = hy > y ? hy : y;
        max_z = hz > z ? hz : z;
    }
};

bool fn_7100e2d4ec(UnkSlotManager_le2d4ec* self, UnkBox_le2d4ec* box) {
    bool any = false;
    for (u32 i = 0; i < 128; ++i) {
        UnkSlot_le2d4ec& slot = self->slots[i];
        if (!slot.active)
            continue;
        f32 ax = slot.x + 100.0f;
        f32 ay = slot.y + 100.0f;
        f32 az = slot.z + 0.0f;
        if (!any) {
            box->min_x = ax;
            box->min_y = ay;
            box->min_z = az;
            box->max_x = slot.x + -100.0f;
            box->max_y = slot.y + -100.0f;
            box->max_z = slot.z;
        } else {
            box->Include(ax, ay, az);
            box->Include(slot.x + -100.0f, slot.y + -100.0f, slot.z);
        }
        any = true;
    }
    return any;
}

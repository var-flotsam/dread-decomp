// CBreakableTileGroup's unit: the tile at a cell of the group's grid, or null off the grid.
// Names and layouts are placeholders.
#include "types.h"

struct UnkTileGroup;

// One tile of a group.
struct UnkQueryObj {
    u32 field_0x0;  // index
    u32 field_0x4;  // type
    u32 field_0x8;  // cell x
    u32 field_0xc;  // cell y
    u8 pad_0x10[0x18];
    f32 field_0x28;  // x
    f32 field_0x2c;  // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;
    u8 pad_0x50[0x58];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

struct UnkTileCollider;

struct UnkTileGroup {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u8 pad_0x15[0x3];
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    u8 pad_0x2c[0x14];
    u32 field_0x40;
    UnkQueryObj* field_0x48;
    u8 pad_0x50[0x8];
    u32* field_0x58;
    u8 pad_0x60[0x68];
    u32 field_0xc8;
    UnkTileCollider* field_0xd0;
};

UnkQueryObj* fn_7100ca1e50(UnkTileGroup* group, s32 x, s32 y) {
    UnkQueryObj* tile = nullptr;
    if (x < 0)
        return tile;
    if (y < 0)
        return tile;
    if (x >= group->field_0x24)
        return tile;
    if (y >= group->field_0x28)
        return nullptr;
    u32 index = group->field_0x58[(u32)(group->field_0x24 * y + x)];
    if (index >= group->field_0x40)
        return nullptr;
    return &group->field_0x48[index];
}

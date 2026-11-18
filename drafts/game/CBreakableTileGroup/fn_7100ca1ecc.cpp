// CBreakableTileGroup's unit: finds the tile under a world position (100 units a cell), stepping back one
// cell when moving in the negative direction onto a cell boundary, and optionally clamping to the grid.
// Names and layouts are placeholders.
#include "game/CBreakableTileGroup.h"
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

// The cell index along one axis, one lower when moving negatively and the coordinate sits on a whole unit.
inline f32 StepBackOnBoundary(f32 cell, f32 coord, f32 dir) {
    if (dir < 0.0f) {
        f32 frac = coord - __builtin_floorf(coord + 0.5f);
        if (frac - 0.001f <= 0.0f) {
            if (frac + 0.001f >= 0.0f)
                cell = cell + -1.0f;
        }
    }
    return cell;
}

inline f32 ClampCell(f32 cell, s32 count) {
    f32 last = (f32)(u32)(count - 1);
    return cell <= 0.0f ? 0.0f : (cell >= last ? last : cell);
}

// The tile in a cell whose column is known to be inside the grid, or null.
inline UnkQueryObj* TileAtCell(UnkTileGroup* group, s32 ix, s32 iy) {
    if (iy >= group->field_0x28)
        return nullptr;
    u32 index = group->field_0x58[(u32)(group->field_0x24 * iy + ix)];
    if (index >= group->field_0x40)
        return nullptr;
    return &group->field_0x48[index];
}

UnkTileQuery_A3_b64* fn_7100ca1ecc(base::reflection::UnkReflObject* object, const void* at, void* arg, bool clamp) {
    UnkTileGroup* group = (UnkTileGroup*)object;
    const f32* pos = (const f32*)at;
    const f32* dir = (const f32*)arg;
    f32 x = pos[0];
    f32 y = pos[1];
    f32 cx = __builtin_floorf((x - group->field_0x18 + 0.001f) / 100.0f);
    f32 cy = __builtin_floorf((y - group->field_0x1c + 0.001f) / 100.0f);

    if (dir) {
        cx = StepBackOnBoundary(cx, x, dir[0]);
        cy = StepBackOnBoundary(cy, y, dir[1]);
    }

    if (clamp) {
        cx = ClampCell(cx, group->field_0x24);
        cy = ClampCell(cy, group->field_0x28);
    }

    UnkQueryObj* tile = nullptr;
    s32 ix = (s32)cx;
    s32 iy;
    if (ix >= 0 && (iy = (s32)cy, iy >= 0) && ix < group->field_0x24)
        tile = TileAtCell(group, ix, iy);
    return (UnkTileQuery_A3_b64*)tile;
}

// CBreakableTileGroup: looks up the tile covering a world position, optionally nudging
// onto the previous cell when moving toward a boundary and clamping to the grid.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBreakableTile {
    u8 pad_0x0[0xa8];
};

struct UnkBreakableTileGroup {
    u8 pad_0x0[0x18];
    f32 originX;
    f32 originY;
    u8 pad_0x20[4];
    s32 width;
    s32 height;
    u8 pad_0x2c[0x14];
    u32 tileCount;
    u8 pad_0x44[4];
    UnkBreakableTile* tiles;
    u8 pad_0x50[8];
    u32* cells;
};

static_assert(__builtin_offsetof(UnkBreakableTileGroup, cells) == 0x58, "cells");
static_assert(__builtin_offsetof(UnkBreakableTileGroup, tiles) == 0x48, "tiles");
static_assert(__builtin_offsetof(UnkBreakableTileGroup, tileCount) == 0x40, "count");

UnkBreakableTile* fn_7100ca1ecc(UnkBreakableTileGroup* self, const f32* pos, const f32* dir, u32 clamp) {
    f32 x = pos[0];
    f32 y = pos[1];
    f32 cx = __builtin_floorf((x - self->originX + 0.001f) / 100.0f);
    f32 cy = __builtin_floorf((y - self->originY + 0.001f) / 100.0f);

    if (dir) {
        if (dir[0] < 0.0f) {
            f32 frac = x - __builtin_floorf(x + 0.5f);
            f32 a = frac + -0.001f;
            f32 b = frac + 0.001f;
            if (b >= 0.0f && a <= 0.0f)
                cx = cx + -1.0f;
        }
        if (dir[1] < 0.0f) {
            f32 frac = y - __builtin_floorf(y + 0.5f);
            f32 a = frac + -0.001f;
            f32 b = frac + 0.001f;
            if (b >= 0.0f && a <= 0.0f)
                cy = cy + -1.0f;
        }
    }

    if (clamp & 1) {
        f32 maxX = (f32)(u32)(self->width - 1);
        f32 maxY = (f32)(u32)(self->height - 1);
        cx = cx > 0.0f ? (cx >= maxX ? maxX : cx) : 0.0f;
        cy = cy > 0.0f ? (cy >= maxY ? maxY : cy) : 0.0f;
    }

    UnkBreakableTile* result = nullptr;
    s32 ix = (s32)cx;
    s32 iy;
    if (ix >= 0 && (iy = (s32)cy, iy >= 0) && ix < self->width) {
        if (self->height <= iy)
            return nullptr;
        u32 index = self->cells[(u32)(self->width * iy + ix)];
        if (index >= self->tileCount)
            return nullptr;
        result = &self->tiles[index];
    }
    return result;
}

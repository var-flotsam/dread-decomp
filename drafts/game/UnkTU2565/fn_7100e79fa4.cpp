// UnkTU2565: collects the indices of the occupied cells of a grid that a rectangle overlaps, up to a
// maximum. Names and layouts are placeholders.
#include "types.h"

struct UnkGridCell_e79fa4 {
    u8 pad_0x0[8];
    u8 occupied;
    u8 pad_0x9[0x3f];
};

struct UnkGrid_e79fa4 {
    UnkGridCell_e79fa4 cells[300][650];
    f32 originX;
    f32 originY;
};

static u32 ToCell(f32 v, f32 lo, f32 hi) {
    f32 c = !(v <= lo) ? (!(v >= hi) ? v : hi) : lo;
    return (u32)__builtin_floorf(c);
}

void fn_7100e79fa4(UnkGrid_e79fa4* grid, const f32* rect, u32 max, u32* count, u32* out) {
    *count = 0;
    f32 sx = rect[0] - grid->originX;
    f32 sy = rect[1] - grid->originY;
    u32 col0 = ToCell((sx + 1.0f) * 0.01f, 0.0f, 649.0f);
    f32 ey = rect[3] - grid->originY;
    f32 ex = rect[2] - grid->originX;
    u32 col1 = ToCell((ex + -1.0f) * 0.01f, (f32)col0, 649.0f);
    u32 row0 = ToCell((sy + 50.0f) * 0.01f, 0.0f, 299.0f);
    u32 row1 = ToCell((ey + -50.0f) * 0.01f, (f32)row0, 299.0f);
    for (u32 row = row0; (row <= row1) & (max != 0); row++) {
        for (u32 col = col0; col <= col1 && *count < max; col++) {
            u32 idx = row * 650 + col;
            if (grid->cells[0][idx].occupied) {
                out[(*count)++] = idx;
            }
        }
    }
}

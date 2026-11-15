// UnkTU2565: whether any active cell of a grid that a rectangle overlaps is in state 6. Names and layouts
// are placeholders.
#include "types.h"

struct UnkGridCell_e79e1c {
    u8 pad_0x0[8];
    u8 state;
    u8 pad_0x9[0x2f];
    u8 active;
    u8 pad_0x39[0xf];
};

struct UnkGrid_e79e1c {
    UnkGridCell_e79e1c cells[300][650];
    f32 originX;
    f32 originY;
};

static u32 ToCell(f32 v, f32 lo, f32 hi) {
    f32 c = !(v <= lo) ? (!(v >= hi) ? v : hi) : lo;
    return (u32)__builtin_floorf(c);
}

bool fn_7100e79e1c(UnkGrid_e79e1c* grid, const f32* rect) {
    f32 sx = rect[0] - grid->originX;
    f32 sy = rect[1] - grid->originY;
    u32 col0 = ToCell((sx + 1.0f) * 0.01f, 0.0f, 649.0f);
    f32 ey = rect[3] - grid->originY;
    f32 ex = rect[2] - grid->originX;
    u32 col1 = ToCell((ex + -1.0f) * 0.01f, (f32)col0, 649.0f);
    u32 row0 = ToCell((sy + 50.0f) * 0.01f, 0.0f, 299.0f);
    u32 row1 = ToCell((ey + -50.0f) * 0.01f, (f32)row0, 299.0f);
    if (row0 > row1)
        return false;
    u32 count = 0;
    for (u32 row = row0; row <= row1; row++) {
        for (u32 col = col0; col <= col1; col++) {
            UnkGridCell_e79e1c& cell = grid->cells[0][row * 650 + col];
            if (cell.active)
                count += cell.state == 6;
        }
    }
    return count > 0;
}

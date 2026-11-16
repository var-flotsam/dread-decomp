// Unit 2565: over the grid cells a rectangle covers (650 x 300 cells of 100 units), whether the share of
// cells whose kind is above 3 reaches a threshold; optionally the largest cell value too.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGridCell_le79c64 {
    u8 pad_0x0[0x8];
    u8 kind;
    u8 pad_0x9[0x3];
    f32 value;
    u8 pad_0x10[0x38];
};
static_assert(sizeof(UnkGridCell_le79c64) == 0x48, "");

struct UnkGrid_le79c64 {
    UnkGridCell_le79c64 cells[300 * 650];
    f32 origin_x;
    f32 origin_y;
};
static_assert(__builtin_offsetof(UnkGrid_le79c64, origin_x) == 0xd63bc0, "");

struct UnkRect_le79c64 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

// The cell coordinate v kept between lo and hi.
inline f32 ClampCell(f32 v, f32 lo, f32 hi) {
    return !(v <= lo) ? (!(v >= hi) ? v : hi) : lo;
}

bool fn_7100e79c64(UnkGrid_le79c64* grid, const UnkRect_le79c64* rect, f32 threshold, f32* max_value) {
    f32 x0 = (rect->min_x - grid->origin_x + 1.0f) * 0.01f;
    u32 col0 = (u32)__builtin_floorf(ClampCell(x0, 0.0f, 649.0f));
    f32 x1 = (rect->max_x - grid->origin_x - 1.0f) * 0.01f;
    f32 x1_clamped = ClampCell(x1, (f32)col0, 649.0f);
    f32 y0 = (rect->min_y - grid->origin_y + 50.0f) * 0.01f;
    u32 row0 = (u32)__builtin_floorf(ClampCell(y0, 0.0f, 299.0f));
    f32 y1 = (rect->max_y - grid->origin_y - 50.0f) * 0.01f;
    u32 row1 = (u32)__builtin_floorf(ClampCell(y1, (f32)row0, 299.0f));
    u32 col1 = (u32)__builtin_floorf(x1_clamped);

    s32 count = 0;
    s32 hits = 0;
    f32 largest = 0.0f;
    for (u32 row = row0; row <= row1; row++) {
        for (u32 col = col0; col <= col1; col++) {
            const UnkGridCell_le79c64& cell = grid->cells[row * 650 + col];
            if (cell.kind > 3)
                hits++;
            if (max_value)
                largest = largest > cell.value ? largest : cell.value;
            count++;
        }
    }
    if (max_value)
        *max_value = largest;
    return (f32)hits / (f32)count >= threshold;
}

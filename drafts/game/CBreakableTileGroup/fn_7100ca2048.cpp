// CBreakableTileGroup's unit (tools/tu_map.py unit 2213): the unbroken tile at a point. A point on a cell
// boundary (within 0.001) also looks in the cell before it along that axis, then in the cell before it
// along both. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

#include <math.h>

using base::math::CVector2D;

extern "C" f32 fmodf(f32, f32);

struct UnkTile_lca2048 {
    u8 pad_0x0[0x38];
    s32 state;  // 0x38: 2 when broken
    u8 pad_0x3c[0x6c];
};
static_assert(sizeof(UnkTile_lca2048) == 0xa8, "tile layout");

struct UnkTileGroup_lca2048 {
    u8 pad_0x0[0x18];
    f32 x;  // 0x18
    f32 y;  // 0x1c
    u8 pad_0x20[0x4];
    s32 width;  // 0x24
    s32 height;  // 0x28
    u8 pad_0x2c[0x14];
    u32 tile_count;  // 0x40
    UnkTile_lca2048* tiles;  // 0x48
    u8 pad_0x50[0x8];
    u32* cells;  // 0x58: the tile index of each cell, row by row
};
static_assert(__builtin_offsetof(UnkTileGroup_lca2048, cells) == 0x58, "group layout");

static const f32 kTileSize = 100.0f;

// Whether `v` is zero, give or take 0.001.
static inline bool NearZero(f32 v) {
    return (v - 0.001f <= 0.0f) & (v + 0.001f >= 0.0f);
}

// The cell of a coordinate along one axis.
static inline f32 CellOf(f32 v, f32 origin) {
    return floorf((v - origin + 0.001f) / kTileSize);
}

// A cell clamped to the grid's `cells` cells.
static inline f32 ClampCell(f32 cell, u32 cells) {
    f32 last = cells - 1;
    f32 clamped = cell >= last ? last : cell;
    if (cell <= 0.0f)
        clamped = 0.0f;
    return clamped;
}

// The cell of a coordinate, one cell back when the coordinate is a whole number and `back` is set.
static inline f32 CellBefore(f32 v, f32 origin, bool back) {
    f32 cell = CellOf(v, origin);
    return back && NearZero(v - floorf(v + 0.5f)) ? cell - 1.0f : cell;
}

// The tile in a cell, null outside the grid or for an empty cell.
static inline UnkTile_lca2048* TileIn(UnkTileGroup_lca2048* group, f32 column, f32 row) {
    s32 c = column;
    if (c < 0)
        return nullptr;
    s32 r = row;
    if (r < 0 || c >= group->width || r >= group->height)
        return nullptr;
    u32 cell = group->width * r + c;
    u32 index = group->cells[cell];
    if (index >= group->tile_count)
        return nullptr;
    return &group->tiles[index];
}

// The tile in the cell before the point along the axes asked for.
static inline UnkTile_lca2048* TileBefore(UnkTileGroup_lca2048* group, const CVector2D* p, bool back_x, bool back_y) {
    f32 column = ClampCell(CellBefore(p->x, group->x, back_x), group->width);
    f32 row = ClampCell(CellBefore(p->y, group->y, back_y), group->height);
    return TileIn(group, column, row);
}

static inline bool Usable(UnkTile_lca2048* tile) {
    return tile && tile->state != 2;
}

UnkQueryObj* fn_7100ca2048(void* observer, void* query, u32 flags) {
    UnkTileGroup_lca2048* group = static_cast<UnkTileGroup_lca2048*>(observer);
    const CVector2D* p = static_cast<const CVector2D*>(query);

    f32 column = CellOf(p->x, group->x);
    f32 row = CellOf(p->y, group->y);
    if (flags & 1) {
        column = ClampCell(column, group->width);
        row = ClampCell(row, group->height);
    }
    UnkTile_lca2048* tile = TileIn(group, column, row);
    // A point on a boundary between cells also belongs to the cell before it.
    bool on_x = false;
    bool on_y = false;
    if (!Usable(tile)) {
        on_x = NearZero(fmodf(p->x, kTileSize));
        if (on_x)
            tile = TileBefore(group, p, true, false);
    }
    if (!Usable(tile)) {
        on_y = NearZero(fmodf(p->y, kTileSize));
        if (on_y)
            tile = TileBefore(group, p, false, true);
    }
    if (!Usable(tile) && on_x && on_y)
        tile = TileBefore(group, p, true, true);
    return reinterpret_cast<UnkQueryObj*>(Usable(tile) ? tile : nullptr);
}

// CBreakableTileGroup's unit: flood-fills the region of cells around (x, y) that are all tiles or
// all empty, marking them in `region` and `visited`; says whether the region gets an outline (a
// tile region always does, an empty one only when it does not touch the grid's border).
// Names and layouts are placeholders.
#include "types.h"

struct UnkTileGroup;

struct UnkGridTile {
    u8 pad_0x0[0x38];
    s32 state;  // 2: broken
    u8 pad_0x3c[0x6c];
};
static_assert(sizeof(UnkGridTile) == 0xa8, "UnkGridTile placeholder layout");

struct UnkTileCells {
    u8 pad_0x0[0x24];
    u32 width;   // 0x24
    u32 height;  // 0x28
    u8 pad_0x2c[0x14];
    u32 tile_count;      // 0x40
    UnkGridTile* tiles;  // 0x48
    u8 pad_0x50[0x8];
    u32* cells;  // 0x58: the tile index of each cell

    // The tile in a cell, if any.
    UnkGridTile* TileAt(s32 x, s32 y) const {
        if (x < 0 || x >= (s32)width || y < 0 || y >= (s32)height)
            return nullptr;
        u32 i = cells[x + y * width];
        if (i >= tile_count)
            return nullptr;
        return &tiles[i];
    }
    // Whether the cell holds a tile that is not broken.
    bool IsTileAt(s32 x, s32 y) const {
        UnkGridTile* tile = TileAt(x, y);
        return tile && tile->state != 2;
    }
    // The same for a cell that may lie past the grid's right or bottom edge.
    bool HasTile(u32 x, u32 y) const {
        if (x >= width || y >= height)
            return false;
        return IsTileAt((s32)x, (s32)y);
    }
};

bool fn_7100ca2f9c(UnkTileGroup* self, bool* region, bool* visited, u32 x, u32 y, bool solid);

bool fn_7100ca2f9c(UnkTileGroup* self, bool* region, bool* visited, u32 x, u32 y, bool solid) {
    UnkTileCells* g = reinterpret_cast<UnkTileCells*>(self);
    // The marks are written as bytes (the grid's size is read again after each one).
    u8* in_region = reinterpret_cast<u8*>(region);
    u8* seen = reinterpret_cast<u8*>(visited);
    in_region[x + y * g->width] = true;
    bool tile = g->IsTileAt((s32)x, (s32)y);
    if ((solid && tile) || (!solid && !tile))
        seen[x + y * g->width] = true;

    bool outlined;
    if (solid)
        outlined = true;
    else
        outlined = x != 0 && y != 0 && x != g->width - 1 && y != g->height - 1;

    // Spreads to a neighbouring cell not seen yet that is of the same kind.
    auto spread = [&](u32 nx, u32 ny) {
        u32 i = nx + ny * g->width;
        if (in_region[i] || seen[i])
            return;
        bool t = g->HasTile(nx, ny);
        if ((solid && t) || (!solid && !t))
            outlined &= fn_7100ca2f9c(self, region, visited, nx, ny, solid);
    };
    if (x != 0)
        spread(x - 1, y);
    if (x < g->width - 1)
        spread(x + 1, y);
    if (y != 0)
        spread(x, y - 1);
    if (y < g->height - 1)
        spread(x, y + 1);
    return outlined;
}

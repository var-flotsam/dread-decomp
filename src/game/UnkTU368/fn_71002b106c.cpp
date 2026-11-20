// Unit 368: whether a navigation cell, and the cell across one of its sides, let something with the
// given flags through; `reason` says which tile flag stopped it. Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU368.h"
#include "game/UnkTU372.h"
#include "types.h"

extern u32 g_unk_7101a21208;  // the flags fn_71002b1270 tests

// The tile a cell lies on: what it blocks.
struct UnkNavTile_b106c {
    u8 pad_0x0[0x1e8];
    bool field_0x1e8;
    bool field_0x1e9;
    bool field_0x1ea;
    u8 pad_0x1eb;
    u16 field_0x1ec;
};

// The part of a cell read here: the tile it lies on (+0x68).
struct UnkNavCellView_b106c {
    u8 pad_0x0[0x68];
    UnkNavTile_b106c* tile;
};

static inline UnkNavTile_b106c* TileOf(UnkNavMeshCell* cell) {
    return reinterpret_cast<UnkNavCellView_b106c*>(cell)->tile;
}

// The cell itself against `flags` (reasons 1 to 4): 1 when it lets them through.
static inline u32 CellAllows(UnkNavMeshCell* cell, u32 flags, u32* reason) {
    u32 allowed = 1;
    if (!TileOf(cell))
        return allowed;
    if ((flags & 1) && TileOf(cell)->field_0x1e9) {
        *reason = 1;
        allowed = 0;
    }
    if ((flags & 2) && TileOf(cell)->field_0x1e9) {
        *reason = 2;
        allowed = 0;
    }
    if ((flags & 0x800) && TileOf(cell)->field_0x1ea) {
        *reason = 3;
        allowed = 0;
    }
    if ((g_unk_7101a21208 & flags) && TileOf(cell)->field_0x1ec != 0 &&
        fn_71002b1270(fn_71002b37d8(TileOf(cell)), flags)) {
        *reason = 4;
        allowed = 0;
    }
    return allowed;
}

bool fn_71002b106c(UnkNavMesh* mesh, u16 cell_id, u8 side, u32 cell_flags, u32 neighbour_flags, u32* reason) {
    bool allowed = true;
    if (cell_flags)
        allowed = CellAllows(fn_710027a328(mesh, cell_id), cell_flags, reason);
    if (!neighbour_flags || side == 0xff)
        return allowed;

    // The cell across the side (reasons 5 to 8).
    UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a328(mesh, cell_id), &side);
    if (edge->field_0x2 == 0xffff || edge->pad_0x4[0] == 0xff)
        return allowed;
    UnkNavMeshCell* other = fn_710027a328(mesh, edge->field_0x2);
    if (!TileOf(other))
        return allowed;
    if ((neighbour_flags & 1) && TileOf(other)->field_0x1e8) {
        *reason = 5;
        allowed = false;
    }
    if ((neighbour_flags & 2) && TileOf(other)->field_0x1e9) {
        *reason = 6;
        allowed = false;
    }
    if ((neighbour_flags & 0x800) && TileOf(other)->field_0x1ea) {
        *reason = 7;
        allowed = false;
    }
    if ((g_unk_7101a21208 & neighbour_flags) && TileOf(other)->field_0x1ec != 0 &&
        fn_71002b1270(fn_71002b37d8(TileOf(other)), cell_flags)) {
        *reason = 8;
        allowed = false;
    }
    return allowed;
}

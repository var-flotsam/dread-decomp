// navmesh::CNavMesh: whether side `*side` of cell `*cell` cannot be crossed with `flags` (the
// crossing test fn_71002afcd8 fails); false for an id out of range. Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

using Cell = UnkNavMeshCell;
using Edge = UnkNavMeshEdge;

bool fn_71002afcd8(Cell*, Edge*, u32);

struct UnkNavMeshCellSlot_710027d100 {
    Cell cell;
    u8 pad[0x70 - sizeof(Cell)];
};

struct UnkNavMesh_710027d100 {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlot_710027d100* cells;  // +0x290
    u32 cell_count;                        // +0x298
};

bool fn_710027d100(UnkNavMesh_710027d100* self, const u16* cell, const u8* side, u32 flags) {
    if (*cell < self->cell_count) {
        Cell* c = &self->cells[*cell].cell;
        if (*side < c->field_0x10)
            return !fn_71002afcd8(c, fn_71002afbe4(c, side), flags);
    }
    return false;
}

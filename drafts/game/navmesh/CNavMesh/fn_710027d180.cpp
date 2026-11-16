// navmesh::CNavMesh: whether going round the corner after side `*side` of cell `*cell` (forwards or
// backwards) through crossable edges leads somewhere other than the start edge's neighbour; true for an id
// out of range or a side that cannot be crossed with `flags`. Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

using Cell = UnkNavMeshCell;
using Edge = UnkNavMeshEdge;

bool fn_71002afcd8(Cell*, Edge*, u32);

struct UnkNavMeshCellSlot_l27d180 {
    Cell cell;
    u8 pad[0x70 - sizeof(Cell)];
};

struct UnkNavMesh_l27d180 {
    u8 pad_0x0[0x290];
    UnkNavMeshCellSlot_l27d180* cells;  // +0x290
    u32 cell_count;                     // +0x298
};

// An edge with the neighbour's side the header leaves in its padding (+0x4).
struct UnkNavMeshEdgeLayout_l27d180 {
    u8 index;
    u8 pad_0x1;
    u16 neighbour;      // +0x2
    u8 neighbour_side;  // +0x4
};

static inline const UnkNavMeshEdgeLayout_l27d180* L(const Edge* e) {
    return reinterpret_cast<const UnkNavMeshEdgeLayout_l27d180*>(e);
}

// The number of sides of a cell: the header's byte is the low byte of a word.
static inline u8 SideCount(const Cell* c) { return (u8)*reinterpret_cast<const u32*>(&c->field_0x10); }

// The next side of `c` going round it forwards or backwards.
static inline u8 StepSide(const Cell* c, u8 side, bool forward) {
    if (forward)
        return SideCount(c) - 1 == side ? 0 : side + 1;
    if (side == 0)
        side = SideCount(c);
    return side - 1;
}

// Crosses into the cell `*cell` at side `*side` and steps round it to the next side: the cell and side on
// the other side of that edge, and whether it can be crossed with `flags`.
static inline bool Cross(UnkNavMesh_l27d180* self, u16* cell, u8* side, bool forward, u32 flags) {
    Cell* c = &self->cells[*cell].cell;
    u8 step = StepSide(c, *side, forward);
    Edge* e = fn_71002afbe4(c, &step);
    bool crossable = fn_71002afcd8(c, e, flags);
    *cell = L(e)->neighbour;
    *side = L(e)->neighbour_side;
    return crossable;
}

bool fn_710027d180(UnkNavMesh_l27d180* self, const u16* cell, const u8* side, bool forward, u32 flags) {
    if (*cell >= self->cell_count)
        return true;
    Cell* c = &self->cells[*cell].cell;
    if (*side >= c->field_0x10)
        return true;
    Edge* start = fn_71002afbe4(c, side);
    if (!fn_71002afcd8(c, start, flags))
        return true;
    u8 next = StepSide(c, *side, forward);
    Edge* edge = fn_71002afbe4(c, &next);
    if (!fn_71002afcd8(c, edge, flags))
        return true;
    // Walk round the corner from neighbour to neighbour until back at the start edge's neighbour or at an
    // edge that cannot be crossed.
    u16 next_cell = L(edge)->neighbour;
    u8 next_side = L(edge)->neighbour_side;
    u16 current;
    do {
        current = next_cell;
        if (start->field_0x2 == current)
            break;
    } while (Cross(self, &next_cell, &next_side, forward, flags));
    return start->field_0x2 != current;
}

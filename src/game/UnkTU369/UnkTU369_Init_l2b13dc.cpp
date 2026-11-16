// Unit 369 (tools/tu_map.py): sets up a per-cell table for a navigation mesh: one empty array for
// each of the mesh's cells, then two groups of three empty arrays and a cleared flag.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/navmesh/CNavMesh.h"
#include "types.h"

using base::global::CRntVector;

struct UnkNavMesh;

struct UnkArrayGroup_l2b13dc {
    CRntVector<void*> first;
    CRntVector<void*> second;
    CRntVector<void*> third;
    bool flag;

    UnkArrayGroup_l2b13dc() : first(0xf), second(0xf), third(0xf), flag(false) {}
};

struct UnkCellTable_l2b13dc {
    CRntVector<CRntVector<void*>> cells;  // 0x0
    UnkArrayGroup_l2b13dc groups[2];      // 0x28
};
static_assert(sizeof(UnkCellTable_l2b13dc) == 0x128, "layout");

using UnkCells_l2b13dc = CRntVector<CRntVector<void*>>;

// Appends an empty array for one cell.
static inline void AppendCell(UnkCells_l2b13dc* cells) {
    CRntVector<void*> cell(0x57);
    u32 n = cells->count;
    u32 new_capacity = cells->capacity;
    if (new_capacity <= n && cells->step != 0)
        new_capacity = (n / cells->step + 1) * cells->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    cells->Reserve(new_capacity);
    u32& count = cells->count;
    new (&cells->data[count]) CRntVector<void*>(static_cast<CRntVector<void*>&&>(cell));
    ++count;
}

void fn_71002b13dc(UnkCellTable_l2b13dc* self, UnkNavMesh* mesh) {
    u32 count = fn_710027a320(mesh);
    new (&self->cells) CRntVector<CRntVector<void*>>(0x57);
    self->cells.Reserve(count);
    new (&self->groups[0]) UnkArrayGroup_l2b13dc();
    new (&self->groups[1]) UnkArrayGroup_l2b13dc();
    for (u32 i = 0; i < fn_710027a320(mesh); ++i)
        AppendCell(&self->cells);
}

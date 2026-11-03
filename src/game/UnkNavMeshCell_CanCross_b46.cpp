// A navigation mesh cell (tools/tu_map.py unit 367, game/UnkNavMeshCell.h): the crossing test
// (fn_71002afcd8) of the cell's edge on side `*index` (the lookup of fn_71002afbe4 inlined).
// Names are placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

using Cell = UnkNavMeshCell;
using Edge = UnkNavMeshEdge;

// Signature as src/game/navmesh/CNavMesh/CNavMesh_SideBlocked.cpp declares it.
bool fn_71002afcd8(Cell*, Edge*, u32);

static inline __attribute__((always_inline)) UnkNavMeshEdge* FindEdge(UnkNavMeshCell* cell, const u8* index) {
    UnkNavMeshEdge* it = cell->edges;
    UnkNavMeshEdge* end = it + cell->edge_count;
    for (; it != end; ++it) {
        UnkNavMeshEdge& edge = *it;
        if (edge.index == *index)
            return &edge;
    }
    return &g_unk_71726a4ae8;
}

bool fn_71002afc84(UnkNavMeshCell* cell, const u8* index, u32 flags) {
    return fn_71002afcd8(cell, FindEdge(cell, index), flags);
}

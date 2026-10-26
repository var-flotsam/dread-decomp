// A navigation-mesh cell (include/game/UnkNavMeshCell.h), translation unit 367 of
// data/tu_map.csv. Only the edge lookups are defined here so far.
#include "game/UnkNavMeshCell.h"

// The first edge record tagged with side `*index`. Naming the record matters: without it Clang
// increments the pointer after the comparison instead of in the load.
UnkNavMeshEdge* fn_71002afbe4(UnkNavMeshCell* cell, const u8* index) {
    UnkNavMeshEdge* it = cell->edges;
    UnkNavMeshEdge* end = it + cell->edge_count;
    for (; it != end; ++it) {
        UnkNavMeshEdge& edge = *it;
        if (edge.index == *index)
            return &edge;
    }
    return &g_unk_71726a4ae8;
}

// The same lookup for a cell that is only read: the original's two copies are byte-identical.
const UnkNavMeshEdge* fn_71002afc34(const UnkNavMeshCell* cell, const u8* index) {
    const UnkNavMeshEdge* it = cell->edges;
    const UnkNavMeshEdge* end = it + cell->edge_count;
    for (; it != end; ++it) {
        const UnkNavMeshEdge& edge = *it;
        if (edge.index == *index)
            return &edge;
    }
    return &g_unk_71726a4ae8;
}

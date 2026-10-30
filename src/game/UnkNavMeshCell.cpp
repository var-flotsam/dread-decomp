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

// The vertex ids at the two ends of side `*index`: its own vertex and the next one (the first for the
// last side).
// NON_MATCHING(minor): register allocation only (the vertex array and i + 1 swap x11 and x12).
UnkNavMeshSidePair fn_71002afe60(UnkNavMeshCell* cell, u8* index) {
    UnkNavMeshSidePair side;
    u8 i = *index;
    if (i != 0xff) {
        u32 count = *reinterpret_cast<const u32*>(&cell->field_0x10);
        if (i < count) {
            u16* vertices = cell->vertices;
            u64 n = (u64)i + 1;
            u64 next = (u32)n < count ? n : 0;
            side.a = vertices[i];
            side.b = vertices[next];
            return side;
        }
    }
    side.raw = 0xffffffff;
    return side;
}

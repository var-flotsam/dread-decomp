// A navigation-mesh cell (include/game/UnkNavMeshCell.h), translation unit 367 of
// data/tu_map.csv. The edge lookups and a side's segment (fn_71002afea8).
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

// ---- fn_71002afea8: a side's segment ----

struct UnkNavMesh;
struct Vec2;
using Mesh = UnkNavMesh;
Vec2* fn_710027a410(Mesh* mesh, u16 index);  // the mesh's vertex (src/game/navmesh/CNavMesh/fn_710027a410.cpp)

// The cell as the side lookup reads it: the vertex ids, their count and the mesh.
struct UnkNavMeshCellSides {
    u8 pad_0x0[0x8];
    u16* vertices;
    u32 count;
    u8 pad_0x14[0x44];
    Mesh* mesh;  // 0x58
};

// NON_MATCHING(minor): the original keeps the result in a register from the start and orders the
// first point's copies and the -1 store differently; same behaviour.
bool fn_71002afea8(const UnkNavMeshCell* cell, const u8* side, UnkNavMeshSideSegment* out) {
    const UnkNavMeshCellSides* c = reinterpret_cast<const UnkNavMeshCellSides*>(cell);
    bool ok = false;
    if (c->mesh) {
        u8 i = *side;
        if (i != 0xff) {
            u32 count = c->count;
            if (i < count) {
                u32 n = i + 1;
                const base::math::CVector2D* a =
                    reinterpret_cast<const base::math::CVector2D*>(fn_710027a410(c->mesh, c->vertices[i]));
                out->start.x = a->x;
                out->field_0x10 = -1.0f;
                out->start.y = a->y;
                u64 next = n < count ? (u64)i + 1 : 0;
                const base::math::CVector2D* b =
                    reinterpret_cast<const base::math::CVector2D*>(fn_710027a410(c->mesh, c->vertices[next]));
                out->end.x = b->x;
                out->end.y = b->y;
                out->field_0x10 = -1.0f;
                ok = true;
            }
        }
    }
    return ok;
}

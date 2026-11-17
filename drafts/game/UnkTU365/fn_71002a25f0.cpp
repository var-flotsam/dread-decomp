// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): whether two polygons' edges meet without
// a reflex turn: each polygon's previous edge must not point into the other's next edge (within a
// thousandth). Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavVertex_l2a25f0 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkNavPolygon_l2a25f0 {
    UnkNavVertex_l2a25f0* vertices;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    UnkSegment2D Edge(u32 i) const {
        return UnkSegment2D(vertices[i].pos, count - 1 == i ? vertices[0].pos : vertices[i + 1].pos);
    }
};

static inline CVector2D Direction(const UnkSegment2D& seg) {
    CVector2D d;
    d.x = seg.start.x - seg.end.x;
    d.y = seg.start.y - seg.end.y;
    f32 length = sqrtf(d.x * d.x + d.y * d.y);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        d.x *= inv;
        d.y *= inv;
    }
    return d;
}

static inline bool Faces(const UnkVec2Result& normal, const CVector2D& d) {
    f32 dot = d.x * normal.x + d.y * normal.y;
    return dot >= 0.0f || (dot + 0.001f >= 0.0f && dot - 0.001f <= 0.0f);
}

bool fn_71002a25f0(void* self, const UnkNavPolygon_l2a25f0* a, u32 edge_a, const UnkNavPolygon_l2a25f0* b,
                   u32 edge_b) {
    if (a == b)
        return false;
    u32 next_a = edge_a == a->EdgeCount() - 1 ? 0 : edge_a + 1;
    u32 next_b = edge_b == b->EdgeCount() - 1 ? 0 : edge_b + 1;
    UnkSegment2D next_edge_a = a->Edge(next_a);
    UnkSegment2D next_edge_b = b->Edge(next_b);
    u32 prev_a = (edge_a == 0 ? a->EdgeCount() : edge_a) - 1;
    u32 prev_b = (edge_b == 0 ? b->EdgeCount() : edge_b) - 1;
    UnkSegment2D prev_edge_a = a->Edge(prev_a);
    UnkSegment2D prev_edge_b = b->Edge(prev_b);
    UnkVec2Result normal_b = fn_7100125e78(&next_edge_b);
    if (!Faces(normal_b, Direction(prev_edge_a)))
        return false;
    UnkVec2Result normal_a = fn_7100125e78(&next_edge_a);
    return Faces(normal_a, Direction(prev_edge_b));
}

// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): whether two portal records join the same
// pair of polygon edges, in either order, with end points within a unit of each other.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavVertex_l2a554c {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkNavPolygon_l2a554c {
    UnkNavVertex_l2a554c* vertices;
    u32 count;

    UnkSegment2D Edge(u32 i) const {
        return UnkSegment2D(vertices[i].pos, count - 1 == i ? vertices[0].pos : vertices[i + 1].pos);
    }
};

static inline const CVector2D& EndOf(const UnkSegment2D& segment) {
    return segment.end;
}

// One side of a portal: an edge of a polygon, or a free point when there is no polygon.
struct UnkNavPortal_l2a554c {
    UnkNavPolygon_l2a554c* polygon_a;
    u32 edge_a;
    UnkNavPolygon_l2a554c* polygon_b;
    u32 edge_b;
    CVector2D point_a;
    CVector2D point_b;

    CVector2D PointA() const { return polygon_a ? EndOf(polygon_a->Edge(edge_a)) : point_a; }
    CVector2D PointB() const { return polygon_b ? EndOf(polygon_b->Edge(edge_b)) : point_b; }
};

// Whether v lies within a unit of target.
static inline bool Within(f32 v, f32 target) {
    return v - 1.0f <= target && v + 1.0f >= target;
}

static inline bool IsNear(const CVector2D& p, const CVector2D& q) {
    return Within(p.x, q.x) && Within(p.y, q.y);
}

bool fn_71002a554c(const UnkNavPortal_l2a554c* a, const UnkNavPortal_l2a554c* b) {
    if (a->polygon_a == b->polygon_a && a->edge_a == b->edge_a && a->polygon_b == b->polygon_b &&
        a->edge_b == b->edge_b) {
        if (IsNear(a->PointA(), b->PointA()) && IsNear(a->PointB(), b->PointB()))
            return true;
    } else if (a->polygon_a == b->polygon_b && a->edge_a == b->edge_b && a->polygon_b == b->polygon_a &&
               a->edge_b == b->edge_a) {
        if (IsNear(a->PointA(), b->PointB()) && IsNear(a->PointB(), b->PointA()))
            return true;
    }
    return false;
}

// Unit 108: whether a polygon turns the same way at every corner: each edge's direction must not
// point against the previous edge's normal by more than a small tolerance. Names and layouts are
// placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolyVertex_l118d4c {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon_l118d4c {
    UnkPolyVertex_l118d4c* points;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    UnkSegment2D Edge(u64 index) const {
        const UnkPolyVertex_l118d4c* next = index == count - 1 ? &points[0] : &points[index + 1];
        return UnkSegment2D(points[index].pos, next->pos);
    }
    // The index of the edge before edge `index`, wrapping around.
    u32 PreviousIndex(u64 index) const { return (index != 0 ? (u32)index : EdgeCount()) - 1; }
    // A zero-length edge at a point.
    UnkSegment2D PointEdge(u64 index) const {
        UnkSegment2D edge = Edge(index);
        return UnkSegment2D(edge.start, edge.start);
    }
};

bool fn_7100118d4c(UnkPolygon_l118d4c* self) {
    for (u64 i = 0; i < self->EdgeCount(); ++i) {
        UnkSegment2D edge = self->Edge(i);
        f32 dy = edge.end.y - edge.start.y;
        f32 dx = edge.end.x - edge.start.x;
        f32 length = sqrtf(dx * dx + dy * dy);
        if (length != 0.0f) {
            dx /= length;
            dy /= length;
        }
        // An open polygon's first edge has no previous edge; a single point stands in for it.
        UnkSegment2D prev = self->closed || i != 0 ? self->Edge(self->PreviousIndex(i)) : self->PointEdge(0);
        UnkVec2Result normal = fn_7100125e78(&prev);
        if (dx * normal.x + dy * normal.y < -0.001f)
            return false;
    }
    return true;
}

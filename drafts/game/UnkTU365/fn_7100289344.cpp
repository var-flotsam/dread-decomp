// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): whether a 2D outline (open or closed)
// turns the same way at every corner: each edge's direction against the previous edge's unit normal.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkOutlinePoint_l289344 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkOutline_l289344 {
    UnkOutlinePoint_l289344* points;  // 0x0
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c

    u32 EdgeCount() const { return !closed && count != 0 ? count - 1 : count; }

    u32 PrevIndex(u32 index) const { return (index == 0 ? EdgeCount() : index) - 1; }

    // The edge from point `index` to the next one, the last point wrapping to the first.
    UnkSegment2D Edge(u32 index) const {
        const UnkOutlinePoint_l289344* next = index == count - 1 ? &points[0] : &points[index + 1];
        return UnkSegment2D(points[index].pos, next->pos);
    }

    // The edge before edge `index`; for the first edge of an open outline, a zero-length one at its start.
    UnkSegment2D PrevEdge(u32 index) const {
        if (index != 0 || closed)
            return Edge(PrevIndex(index));
        UnkSegment2D first = Edge(0);
        return UnkSegment2D(first.start, first.start);
    }
};

bool fn_7100289344(UnkOutline_l289344* self) {
    for (u32 i = 0; i < self->EdgeCount(); ++i) {
        UnkSegment2D edge = self->Edge(i);
        f32 dy = edge.end.y - edge.start.y;
        f32 dx = edge.end.x - edge.start.x;
        f32 length = sqrtf(dx * dx + dy * dy);
        if (length != 0.0f) {
            dx /= length;
            dy /= length;
        }
        UnkSegment2D prev = self->PrevEdge(i);
        UnkVec2Result normal = fn_7100125e78(&prev);
        if (dx * normal.x + dy * normal.y < -0.001f)
            return false;
    }
    return true;
}

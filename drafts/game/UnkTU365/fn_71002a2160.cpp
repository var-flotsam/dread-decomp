// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): the index of a point in an array of
// 2D points, matching within a tenth on each axis; a point not there yet is appended first.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkNavPoint_l2a2160 {
    f32 x;
    f32 y;
    UnkNavPoint_l2a2160(const UnkNavPoint_l2a2160& other) : x(other.x), y(other.y) {}

    // Whether (px, py) is within a tenth of this point on both axes.
    bool IsNear(f32 px, f32 py) const {
        return InRange(px, x - 0.1f, x + 0.1f) && InRange(py, y - 0.1f, y + 0.1f);
    }

    bool InRange(f32 v, f32 low, f32 high) const { return low <= v && high >= v; }
};

struct UnkNavPointOwner_l2a2160 {
    u8 pad_0x0[0x2b8];
    base::global::CRntVector<UnkNavPoint_l2a2160> points;  // 0x2b8
};

struct UnkNavGenerator_l2a2160;

u32 fn_71002a2160(UnkNavGenerator_l2a2160* self, const UnkNavPoint_l2a2160* point, UnkNavPointOwner_l2a2160* owner) {
    base::global::CRntVector<UnkNavPoint_l2a2160>& points = owner->points;
    u32 index = 0;
    for (const UnkNavPoint_l2a2160 *it = points.data, *end = points.data + points.count; it != end; ++it, ++index)
    {
        f32 px = point->x;
        f32 py = point->y;
        if (it->IsNear(px, py))
            return index;
    }
    points.PushBackCopy(*point);
    return points.count - 1;
}

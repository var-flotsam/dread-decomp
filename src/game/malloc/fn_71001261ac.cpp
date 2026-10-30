// Where along a segment (base/math/UnkSegment2D.h) a point projects: 0 at the start, 1 at the end.
#include "base/math/UnkSegment2D.h"

// NON_MATCHING(minor): the two sums take their operands in the other order (same values).
f32 fn_71001261ac(const UnkSegment2D* seg, UnkVec2Result point) {
    f32 dy = seg->end.y - seg->start.y;
    f32 dx = seg->end.x - seg->start.x;
    f32 length = dx * dx + dy * dy;
    f32 py = point.y - seg->start.y;
    f32 px = point.x - seg->start.x;
    return (dy * py + dx * px) / length;
}

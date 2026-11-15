// malloc unit: the point on a segment closest to a point, clamped to the segment's ends.
// Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"

UnkVec2Result fn_710012c6e0(const base::math::CVector2D* point, const UnkSegment2D* seg) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 x = seg->start.x;
    f32 y = seg->start.y;
    f32 lenSq = dx * dx + dy * dy;
    if (!(lenSq < 0x1p-46f)) {
        f32 t = (dx * (point->x - seg->start.x) + dy * (point->y - seg->start.y)) / lenSq;
        t = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
        x = seg->start.x + dx * t;
        y = seg->start.y + dy * t;
    }
    return UnkVec2Result(x, y);
}

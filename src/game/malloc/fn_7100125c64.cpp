// Whether a 2D point lies on a segment within a tolerance: for a degenerate segment, whether it
// is (within 0.001) at the start point; otherwise its projection must fall on the segment
// (or, when `extended`, within the tolerance past either end) and its distance from the
// segment's line must be within the tolerance.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

// The segment's unit normal (fn_7100125e78, inlined).
static inline UnkVec2Result SegmentNormal(const UnkSegment2D* seg) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 len2 = dx * dx + dy * dy;
    if (len2 > 1.1920929e-7f) {
        f32 inv = 1.0f / sqrtf(len2);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        return UnkVec2Result(-ny, nx);
    }
    return UnkVec2Result(0.0f, 0.0f);
}

static inline bool IsNear(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

bool fn_7100125c64(const UnkSegment2D* seg, const CVector2D* p, bool extended, f32 tolerance) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 len2 = dx * dx + dy * dy;
    if (IsNear(len2, 0.0f))
        return IsNear(seg->start.x, p->x) && IsNear(seg->start.y, p->y);
    f32 vx = p->x - seg->start.x;
    f32 vy = p->y - seg->start.y;
    f32 dot = dx * vx + dy * vy;
    f32 t = dot / len2;
    if (extended) {
        if (t > 1.1f || t < -0.1f) {
            f32 len = sqrtf(len2);
            if (dot < -tolerance)
                return false;
            if (dot > len + tolerance)
                return false;
        }
    } else {
        if (t > 1.0f)
            return false;
        if (t < 0.0f)
            return false;
    }
    UnkVec2Result n = SegmentNormal(seg);
    f32 d = vx * n.x + vy * n.y;
    return d >= -tolerance && d <= tolerance;
}

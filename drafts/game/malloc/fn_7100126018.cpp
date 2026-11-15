// Unit 110: projects a point onto a 2D segment; true when the projection falls within the segment
// (widened by the tolerance) or lands on one of its ends. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"

using base::math::CVector2D;

static inline bool IsNear(f32 value, f32 target) {
    return target - 0.001f <= value && value <= target + 0.001f;
}

bool fn_7100126018(const UnkSegment2D* seg, const CVector2D* point, f32 tolerance, CVector2D* out) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 lengthSq = dx * dx + dy * dy;
    f32 t = (dx * (point->x - seg->start.x) + dy * (point->y - seg->start.y)) / lengthSq;
    bool result = (t >= 0.0f - tolerance) & (t <= tolerance + 1.0f);
    if (out != nullptr) {
        if (result) {
            out->x = seg->start.x + t * dx;
            out->y = seg->start.y + t * dy;
        }
    }
    if (!result) {
        f32 px = seg->start.x + t * (seg->end.x - seg->start.x);
        f32 py = seg->start.y + t * (seg->end.y - seg->start.y);
        if (IsNear(px, seg->start.x) && IsNear(py, seg->start.y)) {
            if (out != nullptr) {
                *out = seg->start;
            }
            return true;
        }
        if (!IsNear(px, seg->end.x)) {
            return false;
        }
        result = IsNear(py, seg->end.y);
        if (out != nullptr && result) {
            *out = seg->end;
        }
    }
    return result;
}

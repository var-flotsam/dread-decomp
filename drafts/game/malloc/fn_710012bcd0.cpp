// Unit 110 (2D geometry): a moving point against a segment it may cross from the front. On a hit it
// fills the contact: the crossing point, the travel direction, the segment's normal and the
// fraction of the path. Names and layouts are placeholders.
#include <math.h>

#include "base/math/UnkSegment2D.h"
#include "game/UnkTU110.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSweep2D {
    CVector2D start;
    CVector2D end;
    CVector2D delta;
};

struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;
    u32 flags;

    void SetPoint(f32 x, f32 y) {
        point.x = x;
        point.y = y;
        point.z = 0.0f;
        flags |= 8;
    }
    void SetParam(f32 t) {
        param = t;
        flags |= 0x200;
    }
    void SetNormalA(const CVector2D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const CVector2D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

static const f32 kCrossEpsilon = 1.1920929e-05f;

// The segment's direction turned a quarter turn, pointing to its front.
static inline CVector2D FrontNormal(f32 ex, f32 ey) {
    CVector2D n;
    n.x = ey;
    n.y = -ex;
    return n;
}

bool fn_710012bcd0(const UnkSweep2D* sweep, const UnkSegment2D* seg, UnkContact2D* out) {
    f32 ey = seg->start.y - seg->end.y;
    f32 ex = seg->start.x - seg->end.x;
    f32 dx = sweep->delta.x;
    f32 dy = sweep->delta.y;
    f32 denom = dx * ey - dy * ex;
    CVector2D normal = FrontNormal(ex, ey);
    f32 negDenom = -denom;
    if (!(denom < -kCrossEpsilon)) {
        return false;
    }
    f32 sx = sweep->start.x;
    f32 sy = sweep->start.y;
    f32 wy = sy - seg->end.y;
    f32 wx = sx - seg->end.x;
    f32 num = normal.x * wx + wy * normal.y;
    if (!(num >= 0.0f)) {
        return false;
    }
    f32 lengthSq = dx * dx + dy * dy;
    if (!(num * num <= lengthSq * negDenom * negDenom)) {
        return false;
    }
    f32 u = dy * wx - dx * wy;
    // The crossing must fall between the segment's ends, widened a little either way.
    f32 high = denom * -kCrossEpsilon - denom;
    f32 low = denom * kCrossEpsilon;
    if (!(u >= low && u <= high)) {
        return false;
    }
    f32 t = num / negDenom;
    if (!(t <= 1.0f)) {
        return false;
    }
    out->SetParam(t);
    out->SetPoint(sx + dx * t, sy + dy * t);
    f32 length = sqrtf(lengthSq);
    CVector2D dir;
    dir.x = dx / length;
    dir.y = dy / length;
    out->SetNormalA(dir);
    f32 normalLength = sqrtf(ex * ex + ey * ey);
    if (normalLength != 0.0f) {
        normal.x = normal.x / normalLength;
        normal.y = normal.y / normalLength;
    }
    out->SetNormalB(normal);
    return true;
}

// Unit 110 (2D geometry): a moving point against a circle. On a hit it fills the contact: the point,
// the normal at it, the opposite normal and the fraction of the path. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;  // where along the moving shape's path (0x200)
    u32 flags;  // 8 point, 0x10 normal_b, 0x20 normal_a, 0x200 param

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const CVector3D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = n.z;
        flags |= 0x20;
    }
    void SetNormalB(const CVector3D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = n.z;
        flags |= 0x10;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

struct UnkCircle2D {
    UnkVec2Result center;
    f32 radius;
    UnkCircle2D(const UnkVec2Result& c, f32 r) : center(c), radius(r) {}
};

// A moving point: where it starts, where it ends and the step between them.
struct UnkSweep2D {
    UnkVec2Result start;
    UnkVec2Result end;
    UnkVec2Result delta;
    UnkSweep2D() : start(0.0f, 0.0f), end(0.0f, 0.0f), delta(0.0f, 0.0f) {}
    UnkSweep2D(UnkVec2Result s, UnkVec2Result d)
        : start(s), end(d.x + s.x, d.y + s.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D(const CVector2D& s, const CVector2D& e)
        : start(s.x, s.y), end(e.x, e.y), delta(end.x - start.x, end.y - start.y) {}
    UnkSweep2D& operator=(const UnkSweep2D& other) asm("fn_71001259f4");
};

// The sweep's direction of travel.
UnkVec2Result fn_7100125a64(const UnkSweep2D* sweep);

static const f32 kDegenerateEpsilon = 1.1920929e-11f;

static inline void UnkSetPoint(UnkContact2D* out, f32 x, f32 y) {
    out->point.x = x;
    out->point.y = y;
    out->point.z = 0.0f;
    out->flags |= 8;
}

static inline void UnkSetNormals(UnkContact2D* out, const UnkVec2Result& n, f32 t) {
    out->normal_a.x = n.x;
    out->normal_a.y = n.y;
    out->normal_a.z = 0.0f;
    out->normal_b.x = -n.x;
    out->normal_b.y = -n.y;
    out->normal_b.z = 0.0f;
    out->param = t;
    out->flags |= 0x230;
}

bool fn_710012c840(const UnkSweep2D* sweep, const UnkCircle2D* circle, UnkContact2D* out, s32 mode) {
    UnkVec2Result start = sweep->start;
    f32 dx = sweep->delta.x;
    f32 dy = sweep->delta.y;
    f32 cx;
    f32 cy;
    f32 r;

    // Starting inside the circle is a hit at the start, facing the travel, unless `mode` has bit 1.
    if (!(mode & 2)) {
        f32 r2 = circle->radius * circle->radius;
        cx = circle->center.x;
        cy = circle->center.y;
        r = circle->radius;
        f32 ox = cx - start.x;
        f32 oy = cy - start.y;
        if (ox * ox + oy * oy < r2) {
            UnkSetPoint(out, start.x, start.y);
            UnkSetNormals(out, fn_7100125a64(sweep), 0.0f);
            return true;
        }
    } else {
        cx = circle->center.x;
        cy = circle->center.y;
        r = circle->radius;
    }

    // Solve |start + t * delta - center| = radius for the first t.
    f32 a = dx * dx + dy * dy;
    if (fabsf(a) <= kDegenerateEpsilon)
        return false;
    f32 fx = start.x - cx;
    f32 fy = start.y - cy;
    f32 b = 2.0f * (dx * fx + dy * fy);
    f32 c = fx * fx + fy * fy - r * r;
    f32 disc = b * b - 4.0f * a * c;
    if (disc < 0.0f)
        return false;
    f32 t;
    if (disc == 0.0f) {
        t = -b / (2.0f * a);
    } else {
        f32 root = sqrtf(disc);
        f32 t1 = (root - b) / (2.0f * a);
        f32 t2 = (-b - root) / (2.0f * a);
        t = t1 > t2 ? t2 : t1;
    }
    if (!(t >= 0.0f && t <= 1.0f))
        return false;

    f32 px = dx * t + start.x;
    f32 py = dy * t + start.y;
    f32 nx = circle->center.x - px;
    f32 ny = circle->center.y - py;
    if (!(dx * nx + dy * ny > 0.0f))
        return false;

    UnkSetPoint(out, px, py);
    f32 len = sqrtf(nx * nx + ny * ny);
    if (len != 0.0f) {
        nx = nx / len;
        ny = ny / len;
    }
    UnkSetNormals(out, UnkVec2Result(nx, ny), t);
    return true;
}

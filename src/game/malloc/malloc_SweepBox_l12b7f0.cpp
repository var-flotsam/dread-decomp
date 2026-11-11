// Unit 110 (2D geometry): a moving point against an axis-aligned box (slab clipping). On a hit it
// fills the contact: the entry point, the travel direction, the box face's normal and the
// fraction of the path. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSweep2D {
    CVector2D start;
    CVector2D end;
    CVector2D delta;
};

struct UnkBox2D {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
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

static const f32 kParallelEpsilon = 1.1920929e-07f;
static const f32 kSamePointEpsilon = 1e-05f;
static const f32 kFloatMax = 3.4028235e+38f;

static inline void UnkCopy(CVector2D& dst, const CVector2D& src) {
    dst.x = src.x;
    dst.y = src.y;
}

template <typename T>
static inline void UnkSwap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// Narrows [enter, leave] to the part of the path that lies between `low` and `high` on one axis.
static inline bool UnkClipAxis(f32 start, f32 delta, f32 low, f32 high, f32& enter, f32& leave) {
    if (__builtin_fabsf(delta) < kParallelEpsilon) {
        if (start < low || start > high)
            return false;
    } else {
        f32 inv = 1.0f / delta;
        f32 t1;
        f32 t0 = inv * (low - start);
        t1 = inv * (high - start);
        if (t0 > t1)
            UnkSwap(t0, t1);
        enter = enter > t0 ? enter : t0;
        leave = leave < t1 ? leave : t1;
        if (enter > leave)
            return false;
    }
    return true;
}

static inline bool UnkNear(f32 a, f32 b) {
    return a - kSamePointEpsilon <= b && a + kSamePointEpsilon >= b;
}

static inline void UnkNormalize(CVector2D& v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y);
    if (len != 0.0f) {
        v.x = v.x / len;
        v.y = v.y / len;
    }
}

// NON_MATCHING(minor): the box's x0 and x1 live in swapped registers (s3/s2), the border test
// compares y1 before y0, and the normal's z store is scheduled before the first distance compare.
bool fn_710012b7f0(const UnkSweep2D* seg, const UnkBox2D* box, UnkContact2D* out, s32 mode) {
    CVector2D dir;
    UnkCopy(dir, seg->delta);
    f32 x0 = box->x0;
    f32 y0 = box->y0;
    f32 sx = seg->start.x;
    f32 sy = seg->start.y;
    f32 x1 = box->x1;
    f32 y1 = box->y1;
    f32 enter = 0.0f;
    f32 leave = kFloatMax;
    if (!UnkClipAxis(sx, dir.x, x0, x1, enter, leave))
        return false;
    if (!UnkClipAxis(sy, dir.y, y0, y1, enter, leave))
        return false;
    if (!(enter >= 0.0f && enter <= 1.0f))
        return false;

    f32 px = sx + enter * dir.x;
    f32 py = sy + enter * dir.y;
    if (enter == 0.0f) {
        // Starting inside the box is no hit; starting on its border is one unless `mode` has bit 1.
        if (!(py == y1 || py == y0 || px == x1 || px == x0))
            return false;
        if (UnkNear(px, sx) && UnkNear(py, sy))
            return (mode & 2) == 0;
    }

    // The normal of the nearest face.
    CVector2D face;
    CVector3D normal;
    normal.x = -1.0f;
    normal.y = 0.0f;
    normal.z = 0.0f;
    f32 best = __builtin_fabsf(px - x0);
    f32 dist = __builtin_fabsf(px - x1);
    if (dist < best) {
        best = dist;
        normal.x = 1.0f;
    }
    dist = __builtin_fabsf(py - y0);
    if (dist < best) {
        best = dist;
        normal.x = 0.0f;
        normal.y = -1.0f;
    }
    dist = __builtin_fabsf(py - y1);
    if (dist < best) {
        normal.x = 0.0f;
        normal.y = 1.0f;
    }
    fn_710007b430(&face, &normal);
    // Leaving through that face rather than entering.
    if (!(face.x * dir.x + face.y * dir.y <= 0.0f))
        return false;

    out->SetParam(enter);
    out->SetPoint(px, py);
    UnkNormalize(dir);
    out->SetNormalA(dir);
    out->SetNormalB(face);
    return true;
}

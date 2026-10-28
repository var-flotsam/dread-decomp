// Intersects two 2D segments within a tolerance. Collinear segments (when allowed) intersect
// at a shared endpoint. On a hit, optionally fills a contact: the point (offset by the first
// segment's start) and the two segments' normals.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100126018(const UnkSegment2D* seg, const CVector2D* point, CVector2D* out, f32 tolerance);

struct UnkContact_710012be70 {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
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

static inline CVector2D UnkSub(const CVector2D& l, const CVector2D& r) {
    CVector2D v;
    v.x = l.x - r.x;
    v.y = l.y - r.y;
    return v;
}

static inline CVector2D UnkCopy(const CVector2D& v) {
    CVector2D r;
    r.x = v.x;
    r.y = v.y;
    return r;
}

// NON_MATCHING(minor): the spill of the start point's x is scheduled later, and the two
// products of the denominator get swapped registers.
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out,
                   bool collinear, f32 tolerance) {
    CVector2D s = a->start;
    const CVector2D origin = {0.0f, 0.0f};
    const CVector2D da = UnkSub(a->end, s);
    const CVector2D b0 = UnkSub(b->start, s);
    const CVector2D b1 = UnkSub(b->end, s);
    UnkSegment2D sa(origin, da);
    UnkSegment2D sb(b0, b1);
    f32 dax = da.x;
    f32 day = da.y;
    f32 ny = b1.y - b0.y;
    f32 nx = b0.x - b1.x;
    f32 denom = dax * ny + day * nx;
    f32 t = ny * b0.x + b0.y * nx;
    f32 hx = dax * t / denom;
    f32 hy = day * t / denom;
    if (denom == 0.0f && !collinear)
        return false;
    if (denom == 0.0f && collinear) {
        if (fn_7100125c64(a, &b->start, false, 0.0f) || fn_7100125c64(a, &b->end, false, 0.0f) ||
            fn_7100125c64(b, &a->start, false, 0.0f) || fn_7100125c64(b, &a->end, false, 0.0f)) {
            if (!out)
                return true;
            CVector2D p;
            if (fn_7100125c64(a, &b->start, false, 0.0f))
                p = UnkCopy(b->start);
            else if (fn_7100125c64(a, &b->end, false, 0.0f))
                p = UnkCopy(b->end);
            else if (fn_7100125c64(b, &a->start, false, 0.0f))
                p = UnkCopy(a->start);
            else
                p = UnkCopy(a->end);
            out->SetPoint(CVector3D{s.x + p.x, s.y + p.y, 0.0f});
            out->SetNormalA(fn_7100125e78(&sa));
            out->SetNormalB(fn_7100125e78(&sa));  // the first segment again, as in the original
            return true;
        }
    }
    CVector2D hit = {hx, hy};
    if (!fn_7100126018(&sa, &hit, nullptr, tolerance))
        return false;
    if (!fn_7100126018(&sb, &hit, nullptr, tolerance))
        return false;
    if (!out)
        return true;
    out->SetPoint(CVector3D{s.x + hit.x, s.y + hit.y, 0.0f});
    out->SetNormalA(fn_7100125e78(&sa));
    out->SetNormalB(fn_7100125e78(&sb));
    return true;
}

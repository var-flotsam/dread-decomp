// malloc unit (tools/tu_map.py unit 110): a box moving against an upright capsule. Its two end circles
// are tried first (each only when the hit faces away from the capsule's middle), then the box between
// them, whose contact replaces a circle's only for a hit on one of its sides. Names and layouts are
// placeholders.
#include "base/math/CVectorOps.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The type-8 shape: an upright capsule, its centre, the half height of its straight part and its radius.
struct UnkShape8Geometry;
struct UnkCapsule2D_l12efb0 {
    CVector2D center;
    f32 half_height;
    f32 radius;

    // From the centre to the top circle's centre.
    CVector2D Up() const {
        CVector2D v = {0.0f, half_height};
        return v;
    }
    // The half size of the box between the two circles.
    CVector2D Extent() const {
        CVector2D v = {radius, half_height};
        return v;
    }
};

struct UnkCapsuleCircle {
    CVector2D center;
    f32 radius;
    UnkCapsuleCircle(const CVector2D& c, f32 r) : center(c), radius(r) {}
};

struct UnkCapsuleBox {
    CVector2D min;
    CVector2D max;
};

// A contact as the 2D tests fill it; copied member by member.
struct UnkCapsuleContact {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;
    u32 flags;

    UnkCapsuleContact& operator=(const UnkCapsuleContact& o) {
        point.x = o.point.x;
        point.y = o.point.y;
        point.z = o.point.z;
        normal_a.x = o.normal_a.x;
        normal_a.y = o.normal_a.y;
        normal_a.z = o.normal_a.z;
        normal_b.x = o.normal_b.x;
        normal_b.y = o.normal_b.y;
        normal_b.z = o.normal_b.z;
        param = o.param;
        flags = o.flags;
        return *this;
    }
};

// Whether `x` lies in [low, high].
inline bool InRange(f32 x, f32 low, f32 high) {
    return high >= x && low <= x;
}

inline bool IsNear(f32 v, f32 target) {
    return InRange(target, v - 0.001f, v + 0.001f);
}

// Whether a box normal points along x, to +1 or -1 (`x`).
inline bool IsSideNormal(const CVector3D& n, f32 x) {
    return IsNear(n.x, x) && IsNear(n.y, 0.0f) && IsNear(n.z, 0.0f);
}

static inline bool HitCircle(const void* b, void* x5, const UnkCapsuleCircle& circle, UnkCapsuleContact* out) {
    return fn_710012e5c0(static_cast<const UnkBox2D*>(b), static_cast<const UnkVec2Result*>(x5),
                         reinterpret_cast<const UnkCircle2D*>(&circle), reinterpret_cast<UnkContact2D*>(out));
}

bool fn_710012efb0(const void* b, void* x5, const UnkShape8Geometry* a, UnkContactPoint* out) {
    const UnkCapsule2D_l12efb0* capsule = reinterpret_cast<const UnkCapsule2D_l12efb0*>(a);
    UnkCapsuleContact* result = reinterpret_cast<UnkCapsuleContact*>(out);

    bool hit = false;
    if (HitCircle(b, x5, UnkCapsuleCircle(capsule->center + capsule->Up(), capsule->radius), result) &&
        result->normal_b.y >= 0.0f)
        hit = true;
    else if (HitCircle(b, x5, UnkCapsuleCircle(capsule->center - capsule->Up(), capsule->radius), result) &&
             result->normal_b.y <= 0.0f)
        hit = true;

    UnkCapsuleContact contact = {};
    UnkCapsuleBox box = {capsule->center - capsule->Extent(), capsule->center + capsule->Extent()};
    if (fn_710012dbd0(static_cast<const UnkBox2D*>(b), x5, &box, reinterpret_cast<UnkContactPoint*>(&contact))) {
        // The box's contact wins over a circle's only on the box's left or right side.
        if (!hit || IsSideNormal(contact.normal_a, 1.0f) || IsSideNormal(contact.normal_a, -1.0f)) {
            *result = contact;
            hit = true;
        }
    }
    return hit;
}

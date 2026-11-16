// malloc unit (tools/tu_map.py unit 110): an upright capsule moving against a box. Its two end circles
// are tried first (each only when the hit faces away from the capsule's middle), then the box between
// them, kept only for a hit on one of its sides. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The type-8 shape: an upright capsule, its centre, the half height of its straight part and its radius.
struct UnkShape8Geometry;
struct UnkCapsule2D {
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

bool fn_710012dbd0(const UnkBox2D* a, void* x5, const void* b, UnkContactPoint* out);

static inline CVector2D operator+(const CVector2D& a, const CVector2D& b) {
    CVector2D r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

static inline CVector2D operator-(const CVector2D& a, const CVector2D& b) {
    CVector2D r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

static inline bool IsNear(f32 v, f32 target) {
    return v + 0.001f >= target && v - 0.001f <= target;
}

// Whether a box normal points along x, to +1 or -1 (`x`).
inline bool IsSideNormal(const CVector3D& n, f32 x) {
    return IsNear(n.x, x) && IsNear(n.y, 0.0f) && IsNear(n.z, 0.0f);
}

static inline bool HitCircle(const UnkCapsuleCircle& circle, void* x5, const void* b, UnkCapsuleContact* out) {
    return fn_710012deb0(reinterpret_cast<const UnkCircle2D*>(&circle), static_cast<const UnkVec2Result*>(x5),
                         static_cast<const UnkBox2D*>(b), reinterpret_cast<UnkContact2D*>(out));
}

bool fn_710012ed30(const UnkShape8Geometry* a, void* x5, const void* b, UnkContactPoint* out) {
    const UnkCapsule2D* capsule = reinterpret_cast<const UnkCapsule2D*>(a);
    UnkCapsuleContact* result = reinterpret_cast<UnkCapsuleContact*>(out);

    if (HitCircle(UnkCapsuleCircle(capsule->center + capsule->Up(), capsule->radius), x5, b, result) &&
        result->normal_a.y >= 0.0f)
        return true;
    if (HitCircle(UnkCapsuleCircle(capsule->center - capsule->Up(), capsule->radius), x5, b, result) &&
        result->normal_a.y <= 0.0f)
        return true;

    UnkCapsuleContact contact = {};
    UnkCapsuleBox box = {capsule->center - capsule->Extent(), capsule->center + capsule->Extent()};
    if (!fn_710012dbd0(reinterpret_cast<const UnkBox2D*>(&box), x5, b, reinterpret_cast<UnkContactPoint*>(&contact)))
        return false;
    // Only a hit on the box's left or right side counts.
    if (!IsSideNormal(contact.normal_a, 1.0f) && !IsSideNormal(contact.normal_a, -1.0f))
        return false;
    *result = contact;
    return true;
}

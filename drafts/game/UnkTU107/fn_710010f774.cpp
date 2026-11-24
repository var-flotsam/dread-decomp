// Unit 107: whether a point lies inside a capsule standing on the y axis (its straight part, or either end
// circle). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

// A capsule standing on the y axis: its centre, the half length of its straight part and its radius.
struct UnkCapsule2D {
    UnkVec2Result center;
    f32 half_height;
    f32 radius;
};

struct UnkCircle2D {
    UnkVec2Result center;
    f32 radius;
    UnkCircle2D(const UnkVec2Result& c, f32 r) : center(c), radius(r) {}
};

bool IsCapsuleCentreInsideCircle(const UnkCircle2D* circle, const UnkCapsule2D* capsule, f32 tolerance);

static inline UnkVec2Result Add(const UnkVec2Result& a, const UnkVec2Result& b) {
    return UnkVec2Result(a.x + b.x, a.y + b.y);
}

bool fn_710010f774(const UnkCapsule2D* capsule, const CVector2D* point) {
    f32 x = capsule->center.x;
    f32 y = capsule->center.y;
    f32 half_height = capsule->half_height;
    f32 radius = capsule->radius;
    f32 bottom = y - half_height;
    if (point->x >= x - radius && point->x <= x + radius && point->y >= bottom &&
        point->y <= capsule->half_height + capsule->center.y)
        return true;
    // The second argument is the point: the test reads only the centre a capsule starts with.
    const UnkCapsule2D* at = reinterpret_cast<const UnkCapsule2D*>(point);
    UnkCircle2D lower(UnkVec2Result(x, bottom), radius);
    if (IsCapsuleCentreInsideCircle(&lower, at, 0.0f))
        return true;
    UnkCircle2D upper(Add(capsule->center, UnkVec2Result(0.0f, capsule->half_height)), capsule->radius);
    return IsCapsuleCentreInsideCircle(&upper, at, 0.0f);
}

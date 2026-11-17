// Unit 365 ("navmesh::CNavMeshGenerator"): whether a point may be used between two neighbours: at a
// full weight, a neighbour fails it when the point is not clear of the other point. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkOwner_2a3b0c;

static inline bool NearlyEqual(f32 a, f32 b, f32 tolerance) {
    return a - tolerance <= b && a + tolerance >= b;
}

// Whether `point`, seen from `neighbour`, stays clear of `other`: off the x of `other` when the
// neighbour shares the point's x, off its y when it shares the y, and off `other` itself when it
// shares neither. A neighbour on the point is clear.
static inline bool Clear(const CVector2D& point, const CVector2D& other, const CVector2D& neighbour) {
    bool same_y = NearlyEqual(neighbour.y, point.y, 1e-5f);
    if (NearlyEqual(neighbour.x, point.x, 1e-5f)) {
        if (same_y) {
            return true;
        }
        return !NearlyEqual(other.x, point.x, 1e-5f);
    }
    if (same_y) {
        return !NearlyEqual(other.y, point.y, 1e-5f);
    }
    return !(NearlyEqual(other.x, point.x, 1e-5f) && NearlyEqual(other.y, point.y, 1e-5f));
}

bool fn_71002a3b0c(UnkOwner_2a3b0c* self, const CVector2D* point, const CVector2D* other, const CVector2D* prev,
                   const CVector2D* next, f32 prev_weight, f32 next_weight) {
    if (NearlyEqual(prev_weight, 1.0f, 0.001f) && !Clear(*point, *other, *prev)) {
        return false;
    }
    if (NearlyEqual(next_weight, 1.0f, 0.001f)) {
        return Clear(*point, *other, *next);
    }
    return true;
}

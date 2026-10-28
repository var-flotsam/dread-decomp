// Squared distance from a 2D point to a 2D segment, through the 3D version (z = 0); optionally
// returns the closest point (x and y) and the segment parameter.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// A 3D vector built in place from a 2D one (z = 0) or from three components.
struct UnkVec3_71001262e0 {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3_71001262e0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    explicit UnkVec3_71001262e0(const CVector2D& v) : x(v.x), y(v.y), z(0.0f) {}
};

// Squared distance from p to the segment a-b; closest point and parameter are optional.
f32 fn_7100076640(const UnkVec3_71001262e0* a, const UnkVec3_71001262e0* b, const UnkVec3_71001262e0* p,
                  UnkVec3_71001262e0* closest, f32* t);

struct UnkSegment2D_71001262e0 {
    CVector2D start;
    CVector2D end;
};

f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t) {
    if (closest) {
        UnkVec3_71001262e0 c(0.0f, 0.0f, 0.0f);
        UnkVec3_71001262e0 a(seg->start);
        UnkVec3_71001262e0 b(seg->end);
        UnkVec3_71001262e0 q(*p);
        f32 d = fn_7100076640(&a, &b, &q, &c, t);
        closest->x = c.x;
        closest->y = c.y;
        return d;
    }
    UnkVec3_71001262e0 a(seg->start);
    UnkVec3_71001262e0 b(seg->end);
    UnkVec3_71001262e0 q(*p);
    return fn_7100076640(&a, &b, &q, nullptr, t);
}

// Unit 53: squared distance from a point to the segment a-b; optionally returns the closest point and
// the segment parameter (0 at a, 1 at b). Names and layouts are placeholders.
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

// A plane: points x with dot(normal, x) + dist == 0.
struct UnkPlane_076640 {
    f32 nx;
    f32 ny;
    f32 nz;
    f32 dist;
};

// Signed distance of a point from a plane (scaled by the normal's length).
f32 fn_7100110ca8(const UnkVec3_71001262e0* p, const UnkPlane_076640* plane);

// Copies three floats component by component.
static inline void Copy3(f32* dst, const f32* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

static inline f32 DistSq(const UnkVec3_71001262e0* p, const UnkVec3_71001262e0* q) {
    f32 dx = p->x - q->x;
    f32 dy = p->y - q->y;
    f32 dz = p->z - q->z;
    return dx * dx + dy * dy + dz * dz;
}

f32 fn_7100076640(const UnkVec3_71001262e0* a, const UnkVec3_71001262e0* b, const UnkVec3_71001262e0* p,
                  UnkVec3_71001262e0* closest, f32* t) {
    f32 dx = b->x - a->x;
    f32 dy = b->y - a->y;
    f32 dz = b->z - a->z;
    f32 lenSq = dx * dx + dy * dy + dz * dz;
    if (lenSq == 0.0f) {
        if (closest)
            Copy3(&closest->x, &a->x);
        if (t)
            *t = 0.0f;
        return DistSq(p, a);
    }
    UnkPlane_076640 plane = {dx, dy, dz, -(p->x * dx + p->y * dy + p->z * dz)};
    f32 side = fn_7100110ca8(a, &plane);
    if (side >= 0.0f) {
        if (closest)
            Copy3(&closest->x, &a->x);
        if (t)
            *t = 0.0f;
        return DistSq(p, a);
    }
    f32 along = -side;
    if (lenSq < along) {
        if (closest)
            Copy3(&closest->x, &b->x);
        if (t)
            *t = 1.0f;
        return DistSq(p, b);
    }
    f32 s = along / lenSq;
    UnkVec3_71001262e0 q(s * plane.nx + a->x, s * plane.ny + a->y, s * plane.nz + a->z);
    if (closest)
        *closest = q;
    if (t)
        *t = s;
    return DistSq(p, &q);
}

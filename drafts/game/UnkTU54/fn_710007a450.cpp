// Unit 54: a vector rotated by three angles, about z, then x, then y (the z and x turns are skipped
// when their angles are zero). Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

static inline void SinCos(f32 angle, f32& s, f32& c) {
    s = sinf(angle);
    c = cosf(angle);
}

UnkVec3f fn_710007a450(void* xform, const UnkVec3f* angle) {
    const UnkVec3f& v = *static_cast<const UnkVec3f*>(xform);
    UnkVec3f r;
    if (angle->z == 0.0f) {
        f32 ax = angle->x;
        f32 ay = angle->y;
        if (ax == 0.0f) {
            f32 sy, cy;
            SinCos(ay, sy, cy);
            f32 x = cy * v.x + sy * v.z;
            f32 z = cy * v.z - sy * v.x;
            r.y = v.y;
            r.x = x;
            r.z = z;
        } else {
            UnkVec3f p = v;
            f32 sx, cx, sy, cy;
            SinCos(ax, sx, cx);
            SinCos(ay, sy, cy);
            f32 y1 = p.y * cx - p.z * sx;
            f32 z1 = p.y * sx + p.z * cx;
            r.x = p.x * cy + sy * z1;
            r.y = y1;
            r.z = cy * z1 - p.x * sy;
        }
    } else {
        f32 pz = v.z;
        f32 ax = angle->x;
        f32 px = v.x;
        f32 py = v.y;
        f32 sx, cx, sy, cy, sz, cz;
        SinCos(ax, sx, cx);
        SinCos(angle->y, sy, cy);
        SinCos(angle->z, sz, cz);
        f32 x1 = px * cz - py * sz;
        f32 y1 = px * sz + py * cz;
        f32 y2 = cx * y1 - pz * sx;
        f32 z2 = pz * cx + sx * y1;
        r.x = cy * x1 + sy * z2;
        r.y = y2;
        r.z = cy * z2 - sy * x1;
    }
    return r;
}

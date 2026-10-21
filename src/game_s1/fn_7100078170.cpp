// Transforms a point by a 4x4 float matrix with the perspective divide: the translation column
// is added to each row's dot product and every component is scaled by 1 / w.
#include "types.h"

struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(const UnkVec3f& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkMat4f {
    f32 m[4][4];
};

UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v) {
    f32 x = m->m[0][3] + (v->x * m->m[0][0] + v->y * m->m[0][1] + v->z * m->m[0][2]);
    f32 y = m->m[1][3] + (v->x * m->m[1][0] + v->y * m->m[1][1] + v->z * m->m[1][2]);
    f32 z = m->m[2][3] + (v->x * m->m[2][0] + v->y * m->m[2][1] + v->z * m->m[2][2]);
    f32 w = m->m[3][3] + (v->x * m->m[3][0] + v->y * m->m[3][1] + v->z * m->m[3][2]);
    f32 iw = 1.0f / w;
    UnkVec3f r;
    r.x = x * iw;
    r.y = y * iw;
    r.z = z * iw;
    return r;
}

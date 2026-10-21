// Transforms a 4-component vector by a 4x4 float matrix (each result is a row dotted with the
// vector). The result type has a user-provided copy constructor, so it is returned through the
// hidden result pointer.
#include "types.h"

struct UnkVec4f {
    f32 x, y, z, w;
    UnkVec4f() {}
    UnkVec4f(const UnkVec4f& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkMat4f {
    f32 m[4][4];
};

UnkVec4f fn_7100078218(const UnkMat4f* m, const UnkVec4f* v) {
    UnkVec4f r;
    r.x = v->x * m->m[0][0] + v->y * m->m[0][1] + v->z * m->m[0][2] + v->w * m->m[0][3];
    r.y = v->x * m->m[1][0] + v->y * m->m[1][1] + v->z * m->m[1][2] + v->w * m->m[1][3];
    r.z = v->x * m->m[2][0] + v->y * m->m[2][1] + v->z * m->m[2][2] + v->w * m->m[2][3];
    r.w = v->x * m->m[3][0] + v->y * m->m[3][1] + v->z * m->m[3][2] + v->w * m->m[3][3];
    return r;
}

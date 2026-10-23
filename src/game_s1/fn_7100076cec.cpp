// The scale of an affine matrix (UnkXform, include/game/UnkXformNode.h): the length of each of
// the three basis columns of its 3x3 part. Feeds UnkXformNode's cached scale.
#include "game/UnkXformNode.h"

extern "C" f32 sqrtf(f32);

static inline f32 ColumnLength(const UnkXform* x, int c) {
    f32 a = x->m[0][c];
    f32 b = x->m[1][c];
    f32 d = x->m[2][c];
    return sqrtf(a * a + b * b + d * d);
}

UnkVec3f fn_7100076cec(UnkXform* x) {
    UnkVec3f r;
    r.x = ColumnLength(x, 0);
    r.y = ColumnLength(x, 1);
    r.z = ColumnLength(x, 2);
    return r;
}

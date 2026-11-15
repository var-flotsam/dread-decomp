// Unit 54: the rotation block of a matrix from three angles (identity when all are tiny). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

void fn_7100077a60(UnkMatrix44* mat, f32 a, f32 b, f32 c) {
    f32* m = mat->m;
    if (fabsf(a) > 0.0001f || fabsf(b) > 0.0001f || fabsf(c) > 0.0001f) {
        f32 sc = sinf(c);
        f32 cc = cosf(c);
        f32 sb = sinf(b);
        f32 cb = cosf(b);
        f32 sa = sinf(a);
        f32 ca = cosf(a);
        m[0] = cc * cb;
        f32 sbsa = sb * sa;
        m[2] = sc * sa + cc * (sb * ca);
        m[1] = cc * sbsa - sc * ca;
        m[4] = sc * cb;
        m[5] = sc * sbsa + cc * ca;
        m[6] = sc * (sb * ca) - cc * sa;
        m[8] = -sb;
        m[9] = cb * sa;
        m[10] = cb * ca;
    } else {
        m[0] = 1.0f;
        m[1] = 0.0f;
        m[2] = 0.0f;
        m[4] = 0.0f;
        m[5] = 1.0f;
        m[6] = 0.0f;
        m[8] = 0.0f;
        m[9] = 0.0f;
        m[10] = 1.0f;
    }
}

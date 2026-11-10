#include <math.h>
#include "base/UnkMatrix44.h"
#include "types.h"

// A rotation about the z axis by `angle` radians.
void fn_7100078df0(f32 angle, UnkMatrix44* out) {
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    __builtin_memset(out, 0, sizeof(*out));
    out->m[0] = c;
    out->m[1] = -s;
    out->m[4] = s;
    out->m[5] = c;
    out->m[10] = 1.0f;
    out->m[15] = 1.0f;
}

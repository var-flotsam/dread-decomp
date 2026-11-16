// Unit 54: a matrix with its translation cleared and its rotation columns scaled to unit length
// (a zero-length column is left as it is). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

static inline UnkMatrix44 Scaling(f32 x, f32 y, f32 z) {
    UnkMatrix44 s;
    for (int i = 0; i < 16; i++) s.m[i] = 0.0f;
    s.m[15] = 1.0f;
    s.m[0] = x;
    s.m[5] = y;
    s.m[10] = z;
    return s;
}

static inline f32 Length(f32 x, f32 y, f32 z) {
    return sqrtf(x * x + y * y + z * z);
}

static inline f32 Inverse(f32 len) {
    return len == 0.0f ? 1.0f : 1.0f / len;
}

UnkMatrix44 fn_7100076e30(const UnkMatrix44& src) {
    const f32* m = src.m;
    f32 lx = Length(m[0], m[4], m[8]);
    f32 ly = Length(m[1], m[5], m[9]);
    f32 lz = Length(m[2], m[6], m[10]);
    UnkMatrix44 result;
    UnkCopyMatrix(&result, src);
    result.m[3] = 0.0f;
    result.m[7] = 0.0f;
    result.m[11] = 0.0f;
    UnkCopyMatrix(&result, fn_71000127c8(Scaling(Inverse(lx), Inverse(ly), Inverse(lz)), result));
    return result;
}

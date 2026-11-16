// Unit 19: the 4x4 matrix product a * b (row-major), accumulated element by element into a zeroed
// result. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

UnkMatrix44 fn_71000127c8(const UnkMatrix44& a, const UnkMatrix44& b) {
    UnkMatrix44 r = {};
    for (s32 row = 0; row < 4; ++row)
        for (s32 col = 0; col < 4; ++col)
            for (s32 k = 0; k < 4; ++k)
                r.m[row * 4 + col] += a.m[row * 4 + k] * b.m[k * 4 + col];
    return r;
}

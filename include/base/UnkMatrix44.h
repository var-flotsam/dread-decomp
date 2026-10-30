#pragma once

// A 4x4 matrix of f32, row-major (the transform matrix of UnkXform, game/UnkXformNode.h).
#include "types.h"

struct UnkMatrix44 {
    f32 m[16];
};

// Element-by-element copy, written out: as a loop it would become a memcpy (paired 8-byte moves),
// where the original copies one word at a time.
inline void UnkCopyMatrix(UnkMatrix44* dst, const UnkMatrix44& src) {
    dst->m[0] = src.m[0];
    dst->m[1] = src.m[1];
    dst->m[2] = src.m[2];
    dst->m[3] = src.m[3];
    dst->m[4] = src.m[4];
    dst->m[5] = src.m[5];
    dst->m[6] = src.m[6];
    dst->m[7] = src.m[7];
    dst->m[8] = src.m[8];
    dst->m[9] = src.m[9];
    dst->m[10] = src.m[10];
    dst->m[11] = src.m[11];
    dst->m[12] = src.m[12];
    dst->m[13] = src.m[13];
    dst->m[14] = src.m[14];
    dst->m[15] = src.m[15];
}

// The matrix of a transform (UnkXform, game/UnkXformNode.h, whose matrix is its first member):
// rebuilt from the translation, angles and scale when it is not valid, so the argument is the
// transform itself, not a bare matrix. Defined in src/game_s1/fn_710002614c.cpp.
const UnkMatrix44& fn_710002614c(const UnkMatrix44* m);
// The scale matrix for the three factors. The result is returned through x8.
UnkMatrix44 fn_7100079138(f32 x, f32 y, f32 z);
// The rotation about z by `angle` (radians), returned through x8 (src/game_s1/fn_7100078c9c.cpp).
UnkMatrix44 fn_7100078d8c(f32 angle);
// The matrix product a * b, returned through x8.
UnkMatrix44 fn_71000127c8(const UnkMatrix44& a, const UnkMatrix44& b);

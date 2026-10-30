// A vector through the rotation part of a matrix (unit 54, the math helpers; fn_710007a400 in
// src/game_s1/fn_7100078c9c.cpp calls it).
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"

// `v` through the upper-left 3x3 block of `m` (rows by the vector).
UnkVec3f fn_7100077c04(const UnkMatrix44& m, const UnkVec3f& v) {
    UnkVec3f r;
    r.x = v.x * m.m[0] + v.y * m.m[1] + v.z * m.m[2];
    r.y = v.x * m.m[4] + v.y * m.m[5] + v.z * m.m[6];
    r.z = v.x * m.m[8] + v.y * m.m[9] + v.z * m.m[10];
    return r;
}

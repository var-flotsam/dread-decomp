// A translation matrix: the 4x4 identity with (x, y, z) in elements [3], [7] and [11]. The scale
// matrix counterpart is fn_7100079138. Called by fn_7100aa5e84 and fn_7100aa6a2c.

#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};



UnkMatrix44_7100078c9c fn_7100078c9c(f32 x, f32 y, f32 z) {
    UnkMatrix44_7100078c9c r = {};
    r.m[5] = 1.0f;
    r.m[10] = 1.0f;
    r.m[15] = 1.0f;
    r.m[0] = 1.0f;
    r.m[3] = x;
    r.m[7] = y;
    r.m[11] = z;
    return r;
}

// A vector through a matrix (fn_710007a400): fn_7100077c04 rotates the vector by the matrix
// fn_7100078fc8 derives from the given one.
UnkMatrix44 fn_7100078fc8(const UnkMatrix44& m);
UnkVec3f fn_7100077c04(const UnkMatrix44& m, const UnkVec3f& v);

UnkVec3f fn_710007a400(const UnkVec3f& v, const UnkMatrix44& m) {
    return fn_7100077c04(fn_7100078fc8(m), v);
}

// The rotation about z by `angle` (radians): the 4x4 identity with cos and sin in the upper-left
// 2x2 block.
UnkMatrix44 fn_7100078d8c(f32 angle) {
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    UnkMatrix44 r = {};
    r.m[10] = 1.0f;
    r.m[15] = 1.0f;
    r.m[0] = c;
    r.m[1] = -s;
    r.m[4] = s;
    r.m[5] = c;
    return r;
}

// `v` turned by `angle` (radians) in the plane.
UnkVec2Result fn_710007a2c0(const UnkVec2Result& v, f32 angle) {
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    return UnkVec2Result(c * v.x - s * v.y, s * v.x + c * v.y);
}


// The rotation angles that turn the z axis to `dir`: the yaw about y, the pitch about x, no roll.
UnkVec3f fn_710007a7f0(UnkVec3f dir) {
    UnkVec3f r;
    r.z = 0.0f;
    r.y = atan2f(dir.x, dir.z);
    r.x = atan2f(-dir.y, sqrtf(dir.x * dir.x + dir.z * dir.z));
    return r;
}

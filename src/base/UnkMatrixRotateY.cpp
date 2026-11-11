// From unit 54 of tools/tu_map.py (the matrix helpers of src/game_s1/fn_7100078c9c.cpp): the
// rotation about y by `angle` (radians), the 4x4 identity with cos and sin in the x-z block
// (fn_7100078d1c); also an axis-angle quaternion (fn_7100079e70) and an angle unwrap
// (fn_710007a170).
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkMatrix44 fn_7100078d1c(f32 angle) {
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    UnkMatrix44 r = {};
    r.m[5] = 1.0f;
    r.m[0] = c;
    r.m[2] = s;
    r.m[8] = -s;
    r.m[10] = c;
    r.m[15] = 1.0f;
    return r;
}

// The rotation by `angle` (radians) about the unit `axis` as a quaternion (fn_7100079e70).
struct UnkQuat_7100079e70 {
    f32 x, y, z, w;
};

UnkQuat_7100079e70 fn_7100079e70(const UnkVec3f& axis, f32 angle) {
    f32 half = angle * 0.5f;
    f32 s = sinf(half);
    f32 c = cosf(half);
    UnkQuat_7100079e70 q;
    q.x = s * axis.x;
    q.y = s * axis.y;
    q.z = s * axis.z;
    q.w = c;
    return q;
}

// Moves the angle (radians) by a full turn either way when that brings it closer to `target`
// (fn_710007a170). The angle is the first member of a pair (src/game/CCentralUnitCannonAIComponent/
// fn_71003fd0b0.cpp names the pair UnkAim).
struct UnkAim {
    f32 angle;
    f32 current;
};

void fn_710007a170(UnkAim* aim, f32 target) {
    f32* angle = &aim->angle;
    f32 a = *angle;
    f32 d = fabsf(target - a);
    f32 up = a + 6.2831855f;
    f32 d_up = fabsf(target - up);
    f32 down = a + -6.2831855f;
    f32 d_down = fabsf(target - down);
    f32 best;
    if (d_down < d && d_down < d_up)
        best = down;
    else if (d_up < d && d_up < d_down)
        best = up;
    else
        return;
    *angle = best;
}

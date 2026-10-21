// Right-handed perspective projection matrix from a vertical field of view (radians), aspect
// ratio and near/far distances (depth range 0 to 1, w = -z).
#include "types.h"

extern "C" f32 tanf(f32);

struct UnkMat4f {
    f32 m[4][4];
};

UnkMat4f fn_7100079a04(f32 fov, f32 aspect, f32 near_z, f32 far_z) {
    UnkMat4f r = {};
    f32 f = 1.0f / tanf(fov * 0.5f);
    f32 range = far_z - near_z;
    r.m[0][0] = f / aspect;
    r.m[1][1] = f;
    r.m[2][2] = far_z / range;
    r.m[2][3] = -1.0f;
    r.m[3][2] = (near_z * far_z) / range;
    r.m[3][3] = 0.0f;
    return r;
}

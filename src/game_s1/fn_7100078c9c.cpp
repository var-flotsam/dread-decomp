// A translation matrix: the 4x4 identity with (x, y, z) in elements [3], [7] and [11]. The scale
// matrix counterpart is fn_7100079138. Called by fn_7100aa5e84 and fn_7100aa6a2c.

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

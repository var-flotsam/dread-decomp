// A translation matrix: the 4x4 identity with (x, y, z) in elements [3], [7] and [11]. The scale
// matrix counterpart is fn_7100079138. Called by fn_7100aa5e84 and fn_7100aa6a2c.

#include "base/UnkMatrix44.h"
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

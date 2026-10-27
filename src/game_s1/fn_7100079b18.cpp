// The rotation angles of an affine matrix (UnkXform, include/game/UnkXformNode.h). The
// decomposition fn_7100077038 yields two equivalent angle triples; both are wrapped into
// (-pi, pi] by fn_710007a220. When exactly one has a z angle within 0.001 of `hint` (the
// previously cached z) that one wins, else the one with the smaller sum of absolute angles.
#include "game/UnkXformNode.h"

UnkVec3f fn_710007a220(const UnkVec3f* v);

static inline f32 AbsSum(const UnkVec3f& v) {
    return __builtin_fabsf(v.x) + __builtin_fabsf(v.y) + __builtin_fabsf(v.z);
}

static inline bool Near(f32 v, f32 hint) {
    return hint - 0.001f <= v && hint + 0.001f >= v;
}

// NON_MATCHING(minor): b's x and y swap registers (w8/w9) in the copy-back and the final select.
UnkVec3f fn_7100079b18(UnkXform* x, f32 hint) {
    UnkVec3f a(0.0f, 0.0f, 0.0f);
    UnkVec3f b(0.0f, 0.0f, 0.0f);
    fn_7100077038(x, &a, &b, hint);
    a = fn_710007a220(&a);
    b = fn_710007a220(&b);
    if (!Near(a.z, hint)) {
        if (Near(b.z, hint))
            return b;
    } else if (!Near(b.z, hint)) {
        return a;
    }
    return AbsSum(a) <= AbsSum(b) ? a : b;
}

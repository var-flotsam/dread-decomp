// Unit 2508: scales a vector so its component along an axis is at most a limit (a negative limit
// flips the axis); a vector nearly perpendicular to the axis is returned as it is. Names and
// layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

static inline bool IsNearZero(f32 value) {
    return value - 0.001f <= 0.0f && value + 0.001f >= 0.0f;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkVec3f fn_7100d4dce4(const UnkVec3f& v, const UnkVec3f& axis, f32 limit) {
    UnkVec3f result = v;
    UnkVec3f dir = axis;
    if (limit < 0.0f) {
        limit = -limit;
        dir.x = -dir.x;
        dir.y = -dir.y;
        dir.z = -dir.z;
    }
    f32 along = result.x * dir.x + result.y * dir.y + result.z * dir.z;
    if (!IsNearZero(along)) {
        f32 clamped = !(along <= 0.0f) ? (along >= limit ? limit : along) : 0.0f;
        f32 scale = clamped / along;
        result.x = scale * result.x;
        result.y = scale * result.y;
        result.z = scale * result.z;
    }
    return result;
}
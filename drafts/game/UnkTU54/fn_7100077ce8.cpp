// Unit 54: the rotation of a matrix that turns one chosen axis towards a target point, with a second
// chosen axis kept upright (identity when the two points nearly coincide). Axes are 1 to 3 for x, y
// and z, and 4 to 6 for the same axes negated. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector3D;

// Turns a negated axis (4 to 6) into its positive one; true when it was negated.
static inline bool Unsign(s32& axis) {
    switch (axis) {
    case 5:
        axis = 2;
        return true;
    case 6:
        axis = 3;
        return true;
    case 4:
        axis = 1;
        return true;
    default:
        return false;
    }
}

void fn_7100077ce8(UnkMatrix44* out, const CVector3D* from, const CVector3D* to, s32 aimAxis, s32 upAxis) {
    CVector3D origin = *from;
    CVector3D target = *to;
    CVector3D dir = target - origin;
    f32 length = Length(dir);
    f32* m = out->m;
    if (!(length >= 0.01f)) {
        for (int i = 0; i < 16; i++) m[i] = 0.0f;
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
        return;
    }
    bool aimNegated = Unsign(aimAxis);
    dir = CVector3D{dir.x / length, dir.y / length, dir.z / length};
    bool upNegated = Unsign(upAxis);

    CVector3D side = Cross(dir, CVector3D{0.0f, 1.0f, 0.0f});
    if (aimNegated) {
        dir = -dir;
        side = -side;
    }
    CVector3D up = Cross(dir, side);
    if (upNegated) {
        up = -up;
        side = -side;
    }

    CVector3D right = {0.0f, 0.0f, 0.0f};
    CVector3D forward = {0.0f, 0.0f, 0.0f};
    CVector3D top = {0.0f, 0.0f, 0.0f};
    if (aimAxis == 3) {
        top = dir;
        if (upAxis == 1) {
            right = up;
            forward = -side;
        } else {
            right = side;
            forward = up;
        }
    } else if (aimAxis == 2) {
        forward = dir;
        if (upAxis == 3) {
            right = -side;
            top = up;
        } else {
            right = up;
            top = side;
        }
    } else if (aimAxis == 1) {
        right = dir;
        if (upAxis == 3) {
            forward = side;
            top = up;
        } else {
            forward = up;
            top = -side;
        }
    }

    CVector3D x = Normalized(right);
    CVector3D z = Normalized(forward);
    CVector3D y = Normalized(top);
    m[0] = x.x;
    m[4] = x.y;
    m[8] = x.z;
    m[1] = y.x;
    m[5] = y.y;
    m[9] = y.z;
    m[2] = -z.x;
    m[6] = -z.y;
    m[10] = -z.z;
}

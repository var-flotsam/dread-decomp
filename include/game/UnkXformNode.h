// Transform node with a 3x4 matrix and lazily cached angles and scale (shared by the setters
// in src/game_s1/). Flag bits in flags_0x70: 2 angles valid, 4 scale valid, 8 matrix valid.
#pragma once
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32;

// Copies word by word; the user-provided copy constructor makes returns go through x8.
struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3f(const UnkVec3f& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
    }
    UnkVec3f& operator=(const UnkVec3f& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        return *this;
    }
};

// A transform: a row-major 4x4 matrix (rotation and scale in the upper-left 3x3, translation in
// column 3) with its rotation angles and scale cached lazily. Flag bits: 2 angles valid, 4 scale
// valid, 8 matrix valid. UnkXformNode embeds one at +0x18.
struct UnkXform {
    f32 m[4][4];
    UnkVec3f angle;  // 0x40
    UnkVec3f scale;  // 0x4c
    u32 flags;       // 0x58

    inline UnkVec3f GetTranslation() const;
    inline UnkVec3f GetAngle();
    inline UnkVec3f GetScale();
};
UnkVec3f fn_7100076cec(UnkXform* x);
UnkVec3f fn_7100079b18(UnkXform* x, f32 hint);

UnkVec3f UnkXform::GetTranslation() const {
    return UnkVec3f(m[0][3], m[1][3], m[2][3]);
}

UnkVec3f UnkXform::GetAngle() {
    if (!(flags & 2)) {
        angle = fn_7100079b18(this, angle.z);
        flags |= 2;
    }
    return angle;
}

UnkVec3f UnkXform::GetScale() {
    if (!(flags & 4)) {
        scale = fn_7100076cec(this);
        flags |= 4;
    }
    return scale;
}

#define UNK_CHANGED(a, b) ((a) - 0.001f > (b) || (a) + 0.001f < (b))

struct UnkXformNode {
    u8 pad_0x0[0x18];
    u8 xform_0x18[0x40];
    UnkVec3f angle_0x58;
    UnkVec3f scale_0x64;
    u32 flags_0x70;
    u8 pad_0x74[0x86];
    u8 dirty_0xfa;

    UnkVec3f GetScale(UnkXform* xf) {
        if (!(flags_0x70 & 4)) {
            scale_0x64 = fn_7100076cec(xf);
            flags_0x70 |= 4;
        }
        return scale_0x64;
    }

    UnkVec3f GetAngle(UnkXform* xf) {
        if (!(flags_0x70 & 2)) {
            angle_0x58 = fn_7100079b18(xf, angle_0x58.z);
            flags_0x70 |= 2;
        }
        return angle_0x58;
    }

    UnkVec3f GetScale() {
        if (!(flags_0x70 & 4)) {
            scale_0x64 = fn_7100076cec((UnkXform*)xform_0x18);
            flags_0x70 |= 4;
        }
        return scale_0x64;
    }

    UnkVec3f GetAngle() {
        if (!(flags_0x70 & 2)) {
            angle_0x58 = fn_7100079b18((UnkXform*)xform_0x18, angle_0x58.z);
            flags_0x70 |= 2;
        }
        return angle_0x58;
    }
};

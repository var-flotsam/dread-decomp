// Transform node with a 3x4 matrix and lazily cached angles and scale (shared by the setters
// in src/game_s1/). Flag bits in flags_0x70: 2 angles valid, 4 scale valid, 8 matrix valid.
#pragma once
#include "base/UnkMatrix44.h"
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

    inline const UnkMatrix44& GetMatrix();
    inline UnkVec3f GetTranslation() const;
    inline void SetTranslation(const UnkVec3f& v);
    inline UnkVec3f GetAngle();
    inline UnkVec3f GetScale();
    inline void SetAngle(const UnkVec3f& v);
    inline void SetScale(const UnkVec3f& v);

    UnkXform() {}
    // Word by word, matrix included.
    UnkXform(const UnkXform& o) {
        UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(m), *reinterpret_cast<const UnkMatrix44*>(o.m));
        angle = o.angle;
        scale = o.scale;
        flags = o.flags;
    }
    // From a matrix alone: angles and scale are derived on first use.
    UnkXform(const UnkMatrix44& matrix) {
        *reinterpret_cast<UnkMatrix44*>(m) = matrix;
        angle.x = 0.0f;
        angle.y = 0.0f;
        angle.z = 0.0f;
        scale.x = 0.0f;
        scale.y = 0.0f;
        scale.z = 0.0f;
        flags = 8;
    }
};
UnkVec3f fn_7100076cec(UnkXform* x);
UnkVec3f fn_7100079b18(UnkXform* x, f32 hint);

const UnkMatrix44& fn_710002614c(const UnkMatrix44* m);  // GetMatrix out of line

const UnkMatrix44& UnkXform::GetMatrix() {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(this));
}

UnkVec3f UnkXform::GetTranslation() const {
    return UnkVec3f(m[0][3], m[1][3], m[2][3]);
}

// The value is copied first: the original loads all three components before storing any.
void UnkXform::SetTranslation(const UnkVec3f& v) {
    UnkVec3f t = v;
    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

UnkVec3f UnkXform::GetAngle() {
    if (!(flags & 2)) {
        angle = fn_7100079b18(this, angle.z);
        flags |= 2;
    }
    return angle;
}

// Keeps the scale (derived from the matrix first if only the matrix is valid), stores the
// angles and invalidates the matrix.
void UnkXform::SetAngle(const UnkVec3f& v) {
    if ((flags & 0xc) == 8) {
        scale = fn_7100076cec(this);
        flags |= 4;
    }
    angle = v;
    flags = (flags & ~8u) | 2;
}

// Keeps the angles (derived from the matrix first if only the matrix is valid), stores the
// scale and invalidates the matrix.
void UnkXform::SetScale(const UnkVec3f& v) {
    if ((flags & 0xa) == 8) {
        angle = fn_7100079b18(this, angle.z);
        flags |= 2;
    }
    scale = v;
    flags = (flags & ~0xcu) | 4;
}

UnkVec3f UnkXform::GetScale() {
    if (!(flags & 4)) {
        scale = fn_7100076cec(this);
        flags |= 4;
    }
    return scale;
}

#define UNK_CHANGED(a, b) ((a) - 0.001f > (b) || (a) + 0.001f < (b))

// What a transform node drives (+0x108).
struct UnkXformTarget {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkXformNode {
    u8 pad_0x0[0x18];
    u8 xform_0x18[0x40];
    UnkVec3f angle_0x58;
    UnkVec3f scale_0x64;
    u32 flags_0x70;
    u8 pad_0x74[0x81];
    bool field_0xf5;
    u8 pad_0xf6;
    u8 field_0xf7;
    u8 pad_0xf8;
    bool active_0xf9;
    u8 dirty_0xfa;
    u8 flag_0xfb;
    u8 pad_0xfc[0xc];
    UnkXformTarget* target_0x108;

    UnkXform* Xform() { return reinterpret_cast<UnkXform*>(xform_0x18); }

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
static_assert(sizeof(UnkXformNode) == 0x110, "UnkXformNode layout");

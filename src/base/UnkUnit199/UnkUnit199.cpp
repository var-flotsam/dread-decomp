// Unit 199 of tools/tu_map.py (0x71001695ec-0x710016974c): a perspective's parameters (field of
// view, never below 0, aspect and near and far distances) turned into its projection matrix
// (fn_7100079a04) at +0x60 of a target. Names and layouts are placeholders.
#include "types.h"

struct UnkMat4f {
    f32 m[4][4];
};

UnkMat4f fn_7100079a04(f32 fov, f32 aspect, f32 near_z, f32 far_z);

struct UnkPerspective199 {
    f32 fov;
    f32 aspect;
    f32 near_z;
    f32 far_z;
};

// Copied element by element.
struct UnkProjection199 {
    f32 m[16];

    UnkProjection199& operator=(const UnkMat4f& other) {
        for (u32 i = 0; i < 16; ++i)
            m[i] = other.m[i / 4][i % 4];
        return *this;
    }
};

struct UnkProjectionTarget199 {
    u8 pad_0x0[0x60];
    UnkProjection199 projection;
};

void fn_7100169604(const UnkPerspective199* self, UnkProjectionTarget199* target) {
    target->projection = fn_7100079a04(__builtin_fmaxf(self->fov, 0.0f), self->aspect, self->near_z, self->far_z);
}

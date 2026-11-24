// game::logic::collision::CCapsuleShape2D unit: the capsule's box in the world. Its centre (+0xa8) is
// moved by a transform's translation (+0xc, +0x1c, +0x2c of a 3x4 matrix); the box reaches the radius
// (+0xb4) across and the half height (+0xb0) plus the radius up and down, at the translation's z.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCapsuleBox3_ld60138 {
    CVector3D min;
    CVector3D max;
};

struct UnkCapsuleTransform_ld60138 {
    f32 m[3][4];
};

struct UnkCapsuleShape2D_ld60138 {
    u8 pad_0x0[0xa8];
    f32 center_x;     // 0xa8
    f32 center_y;     // 0xac
    f32 half_height;  // 0xb0
    f32 radius;       // 0xb4
};

UnkCapsuleBox3_ld60138 CCapsuleShape2D_GetTranslatedBoundingBox(const UnkCapsuleShape2D_ld60138* self,
                                     const UnkCapsuleTransform_ld60138* transform) {
    f32 cx = self->center_x, cy = self->center_y;
    f32 half_height = self->half_height, radius = self->radius;
    f32 tx = transform->m[0][3];
    f32 ty = transform->m[1][3];
    f32 z = transform->m[2][3];
    f32 x = cx + tx;
    f32 y = cy + ty;
    f32 extent = radius + half_height;
    return UnkCapsuleBox3_ld60138{{x - radius, y - extent, z}, {x + radius, y + extent, z}};
}

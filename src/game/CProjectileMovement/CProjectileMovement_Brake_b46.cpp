// CProjectileMovement unit (tools/tu_map.py unit 1623): the opposite of the velocity (+0x4b8),
// its x and y scaled by the linked object's factors (+0x120, +0x124) times `t`, each capped at 1.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"

struct UnkProjectileLink4_b46a3 {
    u8 pad_0x0[0x120];
    f32 factor_x;  // 0x120
    f32 factor_y;  // 0x124
};

struct UnkProjectileMovement4_b46a3 {
    u8 pad_0x0[0x4b8];
    f32 vel_x, vel_y, vel_z;  // 0x4b8
    u8 pad_0x4c4[0x104];
    UnkProjectileLink4_b46a3* link;  // 0x5c8
};

UnkVec3f fn_7100ac419c(UnkProjectileMovement4_b46a3* self, f32 t) {
    f32 x = self->vel_x;
    f32 y = self->vel_y;
    f32 z = -self->vel_z;
    UnkVec3f r;
    r.z = z;
    f32 a = self->link->factor_x * t;
    a = a > 1.0f ? 1.0f : a;
    r.x = -(x * a);
    f32 b = self->link->factor_y * t;
    b = b > 1.0f ? 1.0f : b;
    r.y = -(y * b);
    return r;
}

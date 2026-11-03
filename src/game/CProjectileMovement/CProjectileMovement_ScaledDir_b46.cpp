// CProjectileMovement unit (tools/tu_map.py unit 1623): scales the movement's direction by a
// virtual speed (vtable slot 0x2f8) times a factor, stores the velocity and, when the linked
// object's flag is set, records the speed and the absolute direction components there. The
// layouts are placeholders.
#include "types.h"

char* fn_7100ac77c0();

struct UnkProjectileLink_b46a3 {
    u8 pad_0x0[0x130];
    bool enabled;  // 0x130
};

struct UnkProjectileMovement2_b46a3 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 dir_x, dir_y, dir_z;  // 0x40
    u8 pad_0x4c[0x30];
    f32 factor;  // 0x7c
    u8 pad_0x80[0x438];
    f32 vel_x, vel_y, vel_z;  // 0x4b8
    u8 pad_0x4c4[0xf4];
    f32 abs_y;  // 0x5b8
    f32 abs_x;  // 0x5bc
    f32 speed;  // 0x5c0
    u8 pad_0x5c4[4];
    UnkProjectileLink_b46a3* link;  // 0x5c8

    f32 vf_0x2f8() { return reinterpret_cast<f32 (*)(UnkProjectileMovement2_b46a3*)>(vtable[0x2f8 / 8])(this); }
};

void fn_7100ac3880(UnkProjectileMovement2_b46a3* self) {
    fn_7100ac77c0();
    f32 s = self->vf_0x2f8() * self->factor;
    f32 x = self->dir_x;
    f32 y = self->dir_y;
    f32 vx = x * s;
    f32 vy = s * y;
    f32 vz = s * self->dir_z;
    self->vel_x = vx;
    self->vel_y = vy;
    self->vel_z = vz;
    UnkProjectileLink_b46a3* link = self->link;
    if (link && link->enabled) {
        self->speed = s;
        self->abs_y = __builtin_fabsf(y);
        self->abs_x = __builtin_fabsf(x);
    }
}

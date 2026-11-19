// CMovementComponent unit: after the base update, keeps the previous velocity (+0x11c) and sets the velocity
// (+0x10c) and speed (+0x118) from the owner's position moved since the last one (+0xa8/+0xb8/+0xc8) over dt.
// Names and layouts are placeholders.
#include <math.h>

#include "game/CComponent.h"
#include "game/UnkMovementComponent.h"
#include "types.h"

struct UnkMoveActor_l0d1a90 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkMovementComponent {
    u8 pad_0x0[0x30];
    UnkMoveActor_l0d1a90* owner;  // 0x30
    u8 pad_0x38[0x70];
    f32 last_x;  // 0xa8
    u8 pad_0xac[0xc];
    f32 last_y;  // 0xb8
    u8 pad_0xbc[0xc];
    f32 last_z;  // 0xc8
    u8 pad_0xcc[0x40];
    f32 vel_x, vel_y, vel_z;  // 0x10c
    f32 speed;                // 0x118
    f32 prev_vel_x, prev_vel_y, prev_vel_z;  // 0x11c
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010d1a90(UnkMovementComponent* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    self->prev_vel_x = self->vel_x;
    self->prev_vel_y = self->vel_y;
    self->prev_vel_z = self->vel_z;
    f32 speed;
    if (dt > 0.0f) {
        UnkMoveActor_l0d1a90* o = self->owner;
        f32 dx = o->x - self->last_x;
        f32 inv = 1.0f / dt;
        f32 dy = o->y - self->last_y;
        f32 dz = o->z - self->last_z;
        f32 vx = inv * dx;
        f32 vy = inv * dy;
        f32 vz = inv * dz;
        self->vel_x = vx;
        self->vel_y = vy;
        self->vel_z = vz;
        speed = sqrtf(vx * vx + vy * vy + vz * vz);
    } else {
        self->vel_x = 0.0f;
        self->vel_y = 0.0f;
        self->vel_z = 0.0f;
        speed = 0.0f;
    }
    self->speed = speed;
}

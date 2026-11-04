// CDummyMovement unit (1232): the target position is the owner's actor position (+0x230's +0x10, translation at
// +0x44/+0x54/+0x64) plus the offset at +0x238; over a time step above FLT_EPSILON the velocity from the last
// position (+0x84) goes to +0x10c and its length to +0x118. Returns the position. Names and layouts are
// placeholders.
#include <math.h>
#include "types.h"

struct UnkDummyVec_A5_b70 {
    f32 x, y, z;
    UnkDummyVec_A5_b70(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkDummyVec_A5_b70(const UnkDummyVec_A5_b70& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkDummyActor_A5_b70 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkDummyOwner_A5_b70 {
    u8 pad_0x0[0x10];
    UnkDummyActor_A5_b70* actor;
};

struct UnkDummyMovement_A5_b70 {
    u8 pad_0x0[0x84];
    f32 last_x, last_y, last_z;  // 0x84
    u8 pad_0x90[0x7c];
    f32 vel_x, vel_y, vel_z;  // 0x10c
    f32 speed;                // 0x118
    u8 pad_0x11c[0x114];
    UnkDummyOwner_A5_b70* owner;  // 0x230
    f32 off_x, off_y, off_z;      // 0x238
};

UnkDummyVec_A5_b70 fn_71007cf3d8(UnkDummyMovement_A5_b70* self, f32 dt) {
    UnkDummyActor_A5_b70* actor = self->owner->actor;
    UnkDummyVec_A5_b70 pos(actor->x + self->off_x, actor->y + self->off_y, actor->z + self->off_z);
    if (dt > 1.1920929e-7f) {
        f32 dx = pos.x - self->last_x;
        f32 dy = pos.y - self->last_y;
        f32 dz = pos.z - self->last_z;
        f32 inv = 1.0f / dt;
        f32 vx = inv * dx;
        f32 vy = inv * dy;
        f32 vz = inv * dz;
        self->vel_x = vx;
        self->vel_y = vy;
        self->vel_z = vz;
        self->speed = sqrtf(vx * vx + vy * vy + vz * vz);
    }
    return pos;
}

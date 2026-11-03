// CHomingMovement unit: scales the direction at +0x40 by a speed from the tunable at +0x5f8 into the
// velocity at +0x4b8, then hands over to fn_7100ac41e0. Names and layouts are placeholders.
#include "types.h"

struct UnkHomingTunable {
    u8 unk_0x0[0xd4];
    f32 speed;
};

struct UnkHomingMovement {
    u8 unk_0x0[0x40];
    f32 dir[3];
    u8 unk_0x4c[0x4b8 - 0x4c];
    f32 velocity[3];
    u8 unk_0x4c4[0x5f8 - 0x4c4];
    UnkHomingTunable* tunable;
};

void fn_7100ac41e0(UnkHomingMovement* self, void* ctx, f32 dt);

void fn_7100ac216c(UnkHomingMovement* self, void* ctx, f32 dt) {
    f32 speed = self->tunable->speed;
    f32 x = self->dir[0], y = self->dir[1], z = self->dir[2];
    f32 vx = speed * x, vy = speed * y, vz = speed * z;
    self->velocity[0] = vx;
    self->velocity[1] = vy;
    self->velocity[2] = vz;
    fn_7100ac41e0(self, ctx, dt);
}

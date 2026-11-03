// CNailongThornMovementComponent: fn_7100623080 advances the sway (when +0x5e0 is set): the phase at
// +0x5e4 grows by the speed at +0x5ec times `dt` and restarts at zero past 2*pi; the offset at +0x508
// becomes the amplitude at +0x5e8 times its sine, and the direction at +0x4f8 the perpendicular of the
// one at +0x40. Then the base update fn_7100ac3a78 runs. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkNailongThornMovement {
    u8 pad_0x0[0x40];
    f32 dir_x;
    f32 dir_y;
    u8 pad_0x48[0x4f8 - 0x48];
    f32 side_x;
    f32 side_y;
    f32 side_z;
    u8 pad_0x504[0x4];
    f32 offset;
    u8 pad_0x50c[0x5e0 - 0x50c];
    bool swaying;
    u8 pad_0x5e1[0x3];
    f32 phase;
    f32 amplitude;
    f32 speed;
};

void fn_7100ac3a78(UnkNailongThornMovement* self, f32 dt);

void fn_7100623080(UnkNailongThornMovement* self, f32 dt) {
    if (self->swaying) {
        f32 step = self->speed * dt;
        f32 phase = self->phase + step;
        if (phase > 6.2831855f)
            phase = 0.0f;
        self->phase = phase;
        self->offset = self->amplitude * sinf(phase);
        self->side_x = -self->dir_y;
        self->side_y = self->dir_x;
        self->side_z = 0.0f;
    }
    fn_7100ac3a78(self, dt);
}

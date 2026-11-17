// CMetroidCameraCtrl unit: per-frame update that eases the controller's values towards their targets,
// faster the further away they are, or hands over to a virtual reset when asked.
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkMetroidCameraCtrl_l935688;

struct UnkMetroidCameraCtrlVtbl_l935688 {
    u8 pad_0x0[0x148];
    void (*vf_0x148)(UnkMetroidCameraCtrl_l935688* self);
};

struct UnkMetroidCameraCtrl_l935688 {
    const UnkMetroidCameraCtrlVtbl_l935688* vtable;
    u8 pad_0x8[0x230];
    f32 field_0x238;
    f32 field_0x23c;
    f32 field_0x240;
    f32 field_0x244;
    f32 field_0x248;
    f32 field_0x24c;
    f32 field_0x250;
    f32 field_0x254;
    f32 field_0x258;
    u8 pad_0x25c[0x5c];
    f32 field_0x2b8;
    f32 field_0x2bc;
    f32 field_0x2c0;
    f32 field_0x2c4;
    f32 field_0x2c8;
    f32 field_0x2cc;
    u8 pad_0x2d0[0x10];
    f32 field_0x2e0;
    f32 field_0x2e4;
    f32 field_0x2e8;
    f32 field_0x2ec;
    f32 field_0x2f0;
    f32 field_0x2f4;
    f32 field_0x2f8;
};
static_assert(__builtin_offsetof(UnkMetroidCameraCtrl_l935688, field_0x2f8) == 0x2f8, "layout");

void fn_710076438c(UnkMetroidCameraCtrl_l935688* self, bool reset);

// Moves from towards to by t, clamped to [0, 1].
inline f32 Approach_l935688(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

// A speed factor that grows from 1 - weight (no distance) by weight for each unit of distance / range.
inline f32 DistanceFactor_l935688(f32 distance, f32 range, f32 weight) {
    return distance / range * weight + (1.0f - weight);
}

void fn_7100935688(UnkMetroidCameraCtrl_l935688* self, f32 dt, bool reset) {
    fn_710076438c(self, reset);
    if (reset) {
        self->vtable->vf_0x148(self);
        return;
    }

    const f32 t = 5.0f * dt;
    self->field_0x2e4 = Approach_l935688(self->field_0x2e4, self->field_0x2e8, t);
    self->field_0x2ec = Approach_l935688(self->field_0x2ec, self->field_0x2f0, t);
    self->field_0x2f4 = Approach_l935688(self->field_0x2f4, self->field_0x2f8, dt * 0.5f);

    if (!fn_7100dec788(g_unk_7101d47310))
        return;

    f32 factor = DistanceFactor_l935688(fabsf(self->field_0x2b8 - self->field_0x2c4), 900.0f, 0.6f);
    self->field_0x2b8 = Approach_l935688(self->field_0x2b8, self->field_0x2c4, self->field_0x2e4 * dt * factor);

    factor = DistanceFactor_l935688(fabsf(self->field_0x2bc - self->field_0x2c8), self->field_0x2e0 + self->field_0x2e0, 0.3f);
    self->field_0x2bc = Approach_l935688(self->field_0x2bc, self->field_0x2c8, dt * 3.0f * factor);

    factor = DistanceFactor_l935688(fabsf(self->field_0x2c0 - self->field_0x2cc), 360.0f, 0.8f);
    self->field_0x2c0 = Approach_l935688(self->field_0x2c0, self->field_0x2cc, self->field_0x2ec * dt * factor);

    self->field_0x238 =
        Approach_l935688(self->field_0x238, self->field_0x244, fabsf(self->field_0x238 - self->field_0x244) * dt);
    self->field_0x23c = Approach_l935688(self->field_0x23c, self->field_0x248,
                                         self->field_0x2f4 * (fabsf(self->field_0x23c - self->field_0x248) * dt));
    self->field_0x240 =
        Approach_l935688(self->field_0x240, self->field_0x24c, fabsf(self->field_0x240 - self->field_0x24c) * dt);
    self->field_0x254 =
        Approach_l935688(self->field_0x250, self->field_0x258, fabsf(self->field_0x254 - self->field_0x258) * dt);
}

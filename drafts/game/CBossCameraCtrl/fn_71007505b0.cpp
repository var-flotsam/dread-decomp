// CBossCameraCtrl's unit (tools/tu_map.py, unit 1164): the boss camera's per-frame update. After the base
// update it either hands over to a virtual (when asked to snap) or eases the framing position and the
// distance toward their targets and ramps the blend speed up. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct CCameraBoundaryCtrl;
void fn_710076438c(CCameraBoundaryCtrl* self, bool snap, f32 dt);

struct UnkBossCameraCtrl_l7505b0;

struct UnkBossCameraCtrlVtbl_l7505b0 {
    void* slots[41];
    void (*Snap)(UnkBossCameraCtrl_l7505b0* self);  // 0x148
};

struct UnkBossCameraCtrl_l7505b0 {
    UnkBossCameraCtrlVtbl_l7505b0* vtable;
    u8 pad_0x8[0x230];
    f32 pos_x;            // 0x238
    f32 pos_y;            // 0x23c
    f32 pos_z;            // 0x240
    f32 target_x;         // 0x244
    f32 target_y;         // 0x248
    f32 target_z;         // 0x24c
    u8 pad_0x250[4];
    f32 distance;         // 0x254
    f32 target_distance;  // 0x258
    f32 distance_speed;   // 0x25c
    u8 pad_0x260[0xe4];
    f32 speed_scale;      // 0x344
    u8 pad_0x348[0xc];
    f32 blend;            // 0x354
};

static_assert(__builtin_offsetof(UnkBossCameraCtrl_l7505b0, speed_scale) == 0x344, "");
static_assert(__builtin_offsetof(UnkBossCameraCtrl_l7505b0, blend) == 0x354, "");

// Moves `from` toward `to` by the fraction t, clamped to [0, 1].
inline f32 Approach(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return to * t + from * (1.0f - t);
}

void fn_71007505b0(UnkBossCameraCtrl_l7505b0* self, bool snap, f32 dt) {
    fn_710076438c((CCameraBoundaryCtrl*)self, snap, dt);
    if (snap) {
        self->vtable->Snap(self);
        return;
    }

    f32 blend = self->blend;
    f32 distance_rate = self->distance_speed * (self->speed_scale * dt * 5e-05f);
    f32 x = Approach(self->pos_x, self->target_x, dt * 0.5f * blend * fabsf(self->pos_x - self->target_x));
    f32 y = Approach(self->pos_y, self->target_y, dt * 0.25f * blend * fabsf(self->pos_y - self->target_y));
    f32 z = Approach(self->pos_z, self->target_z, blend * dt * fabsf(self->pos_z - self->target_z));
    f32 distance = self->distance;
    f32 target_distance = self->target_distance;
    self->pos_x = x;
    self->pos_y = y;
    self->pos_z = z;
    self->distance = Approach(distance, target_distance, fabsf(distance - target_distance) * distance_rate);

    blend = blend * 1.025f;
    self->blend = blend > 0.0f ? fminf(blend, 1.0f) : 0.0f;
}

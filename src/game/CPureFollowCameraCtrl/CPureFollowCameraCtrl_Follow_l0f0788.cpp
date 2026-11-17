// CPureFollowCameraCtrl unit (tools/tu_map.py, unit 2695): after the base update (fn_710076438c), either
// snaps through the vtable's +0x148, or eases the followed position (+0x238) towards its target (+0x244),
// each axis by a weight that grows with its distance (the y axis at 0.075 of the rate). Names and layouts
// are placeholders.
#include <math.h>

#include "game/UnkXformNode.h"
#include "types.h"

struct CCameraBoundaryCtrl;
// The flag goes to the base update as the original passes it.
void fn_710076438c(CCameraBoundaryCtrl* self, f32 dt, bool snap);

struct UnkPureFollowCameraCtrl_l0f0788 {
    void** vtable;
    u8 pad_0x8[0x230];
    UnkVec3f position;  // 0x238
    UnkVec3f target;  // 0x244

    void vf_0x148() { reinterpret_cast<void (*)(UnkPureFollowCameraCtrl_l0f0788*)>(vtable[0x148 / 8])(this); }
};

// `from` moved towards `to` by the weight `t`, clamped to [0, 1].
static inline f32 Approach(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    f32 kept = from * (1.0f - t);
    return to * t + kept;
}

void fn_71010f0788(UnkPureFollowCameraCtrl_l0f0788* self, f32 dt, bool snap) {
    fn_710076438c(reinterpret_cast<CCameraBoundaryCtrl*>(self), dt, snap);
    if (snap) {
        self->vf_0x148();
        return;
    }
    self->position.x = Approach(self->position.x, self->target.x, fabsf(self->position.x - self->target.x) * dt);
    self->position.y =
        Approach(self->position.y, self->target.y, dt * 0.075f * fabsf(self->position.y - self->target.y));
    self->position.z = Approach(self->position.z, self->target.z, fabsf(self->position.z - self->target.z) * dt);
}

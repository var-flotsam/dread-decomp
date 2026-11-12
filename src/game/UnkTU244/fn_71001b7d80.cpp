#include <math.h>

#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCam_b0258i {
    u8 pad_0x0[0x74];
    f32 field_0x74;
    f32 tan_0x78;
    f32 tan_0x7c;
    f32 fov;  // 0x80
    f32 field_0x84;
    f32 far_plane;   // 0x88
    f32 near_plane;  // 0x8c
    u8 pad_0x90[0x4];
    f32 field_0x94;
    f32 field_0x98;
    f32 distance;  // 0x9c
    u8 pad_0xa0[0x8];
    UnkVec3f eye;  // 0xa8
    UnkVec3f dir;  // 0xb4
    UnkVec3f up;   // 0xc0
    UnkVec3f right;  // 0xcc
    UnkVec3f target;  // 0xd8
    u8 pad_0xe4[0x184];
    f32 sin_0x268;
    f32 cos_0x26c;
    f32 inv_0x270;
    f32 cos2_0x274;
    f32 tan_0x278;
};
static_assert(__builtin_offsetof(UnkCam_b0258i, tan_0x278) == 0x278, "layout");

f32 fn_7100217ee0(f32 f0, f32 f1);

static inline f32 Rad_b0258i(f32 deg) {
    return (f32)((f64)deg * 3.141592653589793 / 180.0);
}

static inline __attribute__((always_inline)) void SetFov_b0258i(UnkCam_b0258i* self, f32 fov) {
    self->fov = fov;
    f32 half = Rad_b0258i(fov) * 0.5f + 0.017453292f;
    self->sin_0x268 = sinf(half);
    f32 c = cosf(half);
    self->cos_0x26c = c;
    f32 inv = 1.0f / c;
    self->cos2_0x274 = c * c;
    self->inv_0x270 = inv;
    self->tan_0x278 = inv * self->sin_0x268;
    f32 h = Rad_b0258i(fov * 0.5f);
    self->tan_0x7c = sinf(h) / cosf(h);
    f32 r = Rad_b0258i(fn_7100217ee0(self->fov, self->field_0x94));
    self->field_0x84 = r;
    f32 h2 = self->field_0x98 * (r * 0.5f);
    self->tan_0x78 = sinf(h2) / cosf(h2);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers.
void fn_71001b7d80(UnkCam_b0258i* self, f32 fov) {
    SetFov_b0258i(self, fov);
}


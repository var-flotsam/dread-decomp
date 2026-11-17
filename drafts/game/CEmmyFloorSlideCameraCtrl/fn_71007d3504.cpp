// CEmmyFloorSlideCameraCtrl's unit (tools/tu_map.py unit 1237): the blend update. After vtable slot 0x48
// (with false), while the time (+0x1f4) has not passed the duration (+0x1f8), moves the position (+0x38)
// from where it started (+0x1dc) along the "s" curve (fn_7100076020) and the direction (+0x44) from its
// start (+0x1e8) linearly, keeps the direction normalised and runs the time on. Names and layouts are
// placeholders.
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCamDir_l7d3504 {
    f32 x, y, z;
};

struct UnkFloorSlideCamera_l7d3504 {
    void** vtable;
    u8 pad_0x8[0x38 - 0x8];
    UnkVec3f pos;  // 0x38
    UnkCamDir_l7d3504 dir;  // 0x44
    u8 pad_0x50[0x1dc - 0x50];
    UnkVec3f start_pos;  // 0x1dc
    UnkCamDir_l7d3504 start_dir;  // 0x1e8
    f32 time;            // 0x1f4
    f32 duration;        // 0x1f8

    void vf_0x48(bool on) { reinterpret_cast<void (*)(UnkFloorSlideCamera_l7d3504*, bool)>(vtable[0x48 / 8])(this, on); }
};
static_assert(__builtin_offsetof(UnkFloorSlideCamera_l7d3504, duration) == 0x1f8, "layout");

// From `from` to `to` by `t`; `to` itself from 1 on.
static inline UnkVec3f Lerp(const UnkVec3f& from, const UnkVec3f& to, f32 t) {
    if (t >= 1.0f)
        return to;
    f32 u = 1.0f - t;
    f32 ax = u * from.x, ay = u * from.y, az = u * from.z;
    f32 bx = t * to.x, by = t * to.y, bz = t * to.z;
    return UnkVec3f(ax + bx, ay + by, az + bz);
}

static inline UnkCamDir_l7d3504 Lerp(const UnkCamDir_l7d3504& from, const UnkCamDir_l7d3504& to, f32 t) {
    if (t >= 1.0f)
        return to;
    f32 u = 1.0f - t;
    f32 ax = u * from.x, ay = u * from.y, az = u * from.z;
    f32 bx = t * to.x, by = t * to.y, bz = t * to.z;
    return {ax + bx, ay + by, az + bz};
}

void fn_71007d3504(UnkFloorSlideCamera_l7d3504* self, f32 dt) {
    self->vf_0x48(false);
    if (!(self->time <= self->duration))
        return;
    f32 t = self->time / self->duration;
    f32 s = fn_7100076020(t, "s");
    self->pos = Lerp(self->start_pos, self->pos, s);
    self->dir = Lerp(self->start_dir, self->dir, t);
    UnkCamDir_l7d3504& d = self->dir;
    f32 inv = 1.0f / (__builtin_sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    d.x = d.x * inv;
    d.y = inv * d.y;
    d.z = inv * d.z;
    self->time += dt;
}

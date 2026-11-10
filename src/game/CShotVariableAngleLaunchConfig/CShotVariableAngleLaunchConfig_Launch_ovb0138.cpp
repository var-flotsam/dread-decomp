// CShotVariableAngleLaunchConfig's unit (tools/tu_map.py): the launch set-up. After the base set-up
// (fn_71009fa098, src/game/CShotLaunchConfig/CShotLaunchConfig_Launch_ovb0090.cpp) it picks the launch
// speed (+0x1c, or +0x2c up close, blended between the two distances at +0x30 and +0x34 when +0x28 is
// set) and solves the angle between the two limits (+0x20, +0x24, in degrees) with fn_71009fed3c.
// Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShotLaunch_ovb0090;
bool fn_71009fa098(void* owner, const UnkVec3f* target, const UnkVec3f* direction, const UnkVec3f* origin, bool a,
                   bool b, UnkShotLaunch_ovb0090* out, f32 value);

struct UnkVariableAngleConfig_ovb0138 {
    u8 pad_0x0[0x8];
    f32 field_0x8;
    u8 pad_0xc[0xc];
    f32 field_0x18;
    f32 speed;          // 0x1c
    f32 min_angle;      // 0x20, degrees
    f32 max_angle;      // 0x24, degrees
    bool by_distance;   // 0x28
    f32 near_speed;     // 0x2c
    f32 near_dist;      // 0x30
    f32 far_dist;       // 0x34
};

struct UnkVariableAngleLaunch_ovb0138 {
    u8 pad_0x0[0x24];
    u8 field_0x24[4];
    f32 speed;       // 0x28
    f32 field_0x2c;
    u8 pad_0x30[0x18];
    bool launched;   // 0x48
    u8 pad_0x49[7];
    u8 field_0x50[8];
};

bool fn_71009fed3c(UnkVariableAngleConfig_ovb0138* self, const UnkVec3f* target, const UnkVec3f* direction,
                   const UnkVec3f* origin, void* out_a, void* out_b, bool c, bool a, bool b, f32 f0, f32 speed,
                   f32 min_angle, f32 max_angle, f32 value, f32 f8);

static inline f32 Lerp(f32 t, f32 a, f32 b) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return (1.0f - t) * a + t * b;
}

bool fn_71009feb78(UnkVariableAngleConfig_ovb0138* self, const UnkVec3f* target, const UnkVec3f* direction,
                   const UnkVec3f* origin, bool a, bool b, UnkVariableAngleLaunch_ovb0138* out, f32 value) {
    if (!fn_71009fa098(self, target, direction, origin, a, b, reinterpret_cast<UnkShotLaunch_ovb0090*>(out), value))
        return false;
    f32 f18 = self->field_0x18;
    out->field_0x2c = f18;
    __builtin_memcpy(&out->speed, &self->speed, sizeof(f32));
    if (self->by_distance) {
        f32 near = self->near_dist * self->near_dist;
        f32 dy = direction->y - target->y;
        f32 d = __builtin_fabsf(direction->x - target->x) + dy * (dy > 0.0f ? 2.0f : 0.5f);
        d = d > 0.0f ? d : 0.0f;
        d = d * d;
        if (d < near) {
            out->speed = self->near_speed;
        } else {
            f32 far = self->far_dist * self->far_dist;
            if (d < far)
                out->speed = Lerp((d - near) / (far - near), self->near_speed, self->speed);
        }
    }
    f32 min_angle = (f32)((f64)self->min_angle * 3.14159265358979323846 / 180.0);
    f32 max_angle = (f32)((f64)self->max_angle * 3.14159265358979323846 / 180.0);
    if (!fn_71009fed3c(self, target, direction, origin, out->field_0x24, out->field_0x50, true, a, b,
                       f18, out->speed, min_angle, max_angle, value, self->field_0x8))
        return false;
    out->launched = true;
    return true;
}

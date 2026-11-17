// CCameraFX_RotationShake's unit (tools/tu_map.py unit 2700): applies the rotation shake to a camera. While not
// finished it tilts the camera's angles about its shake axis, by the curve at the elapsed fraction times the
// shake's strength, keeping the camera at its distance from the origin when there is a player, then runs the
// base shake. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU53.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CCameraFX_Shake/CCameraFX_Shake_Falloff_ovb0108.cpp defines it.
struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;
f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale);

// As src/game/CCharClassDropItemEvent/CCharClassDropItemEvent_drop_ovo0311.cpp declares it.
UnkVec3f fn_710007a450(void* xform, const UnkVec3f* v);

struct UnkRotationShake_l0f323c;
void fn_71010f3eb0(UnkRotationShake_l0f323c* self, void* camera, UnkVec3f* pos, UnkVec3f* target, void* arg4,
                   void* arg5, f32 dt);

struct UnkRotationShake_l0f323c {
    u8 pad_0x0[0x18];
    f32 yaw, pitch, roll;  // 0x18: the shake axis, in degrees
    u8 pad_0x24[0x10];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 elapsed;     // 0x50
    bool finished;   // 0x54
    u8 pad_0x55[3];
    f32 strength;    // 0x58
    u8 pad_0x5c[4];
    const char* curve;  // 0x60
    u8 pad_0x68[0x28];
    bool keep_distance;  // 0x90
};

static inline f32 Saturate(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

// `v` turned by the angles.
inline UnkVec3f Rotate(const UnkVec3f& v, const UnkVec3f& angles) {
    return fn_710007a450(const_cast<UnkVec3f*>(&v), &angles);
}

// fn_710007a7f0 reads its argument in place (it is passed by address): the angles of `dir` without a copy.
using UnkAnglesFn_l0f323c = UnkVec3f (*)(const UnkVec3f& dir);

static const f32 kDegToRad = 0.017453292f;

void fn_71010f323c(UnkRotationShake_l0f323c* self, void* camera, UnkVec3f* pos, UnkVec3f* rot, void* arg4,
                   void* arg5, f32 dt) {
    if (!self->finished) {
        f32 weight = 1.0f;
        if (self->curve && *self->curve)
            weight = fn_7100076020(Saturate(self->elapsed / self->duration), self->curve);
        f32 amount = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                                   reinterpret_cast<const UnkShakePos_ovb0108*>(pos), weight);
        if (self->strength > 0.0f) {
            f32 x = self->pitch;
            f32 y = self->yaw;
            f32 z = self->roll;
            f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
            f32 scale = amount * self->strength;
            f32 nx = x * inv;
            f32 ny = y * inv;
            f32 nz = z * inv;
            f32 sx = scale * nx;
            f32 sy = scale * ny;
            f32 sz = scale * nz;
            UnkVec3f angles = reinterpret_cast<UnkAnglesFn_l0f323c>(fn_710007a7f0)(*rot);
            UnkVec3f v = Rotate(UnkVec3f(0.0f, 0.0f, 1.0f),
                                UnkVec3f(sx * -kDegToRad, sy * -kDegToRad, sz * kDegToRad));
            v = Rotate(v, angles);
            if (self->keep_distance && fn_7100dec788(g_unk_7101d47310)) {
                f32 dx = 0.0f - pos->x;
                f32 dy = 0.0f - pos->y;
                f32 dz = 0.0f - pos->z;
                f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);
                pos->x = 0.0f - distance * v.x;
                pos->y = 0.0f - distance * v.y;
                pos->z = 0.0f - distance * v.z;
            }
            *rot = v;
        }
    }
    fn_71010f3eb0(self, camera, pos, pos, arg4, arg5, dt);
}

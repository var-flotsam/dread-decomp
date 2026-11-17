// CCameraFX_OscillationShake unit (tools/tu_map.py, unit 1170): the per-frame update. While the shake runs,
// each of its three axes swings its phase back and forth, picking a new random amplitude each time it turns,
// and the camera position moves along the side, up and view directions by the phases, scaled by the curve's
// strength; then the base update runs. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU53.h"
#include "types.h"

extern "C" int rand();

using base::math::CVector3D;

struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;
f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale);

struct UnkOscillationShake_l767ba0;
void fn_71010f3eb0(UnkOscillationShake_l767ba0* self, void* camera, CVector3D* pos, const CVector3D* dir, void* a4,
                   void* a5, f32 dt);

struct UnkOscillationCurve_l767ba0 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    u32 use_count;
};

struct UnkOscillationShake_l767ba0 {
    u8 pad_0x0[0x34];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 time;  // 0x50
    bool stopped;  // 0x54
    u8 pad_0x55[0x3];
    UnkOscillationCurve_l767ba0* curve;  // 0x58
    u8 pad_0x60[0x18];
    f32 period;        // 0x78
    f32 amplitude[3];  // 0x7c
    f32 speed[3];      // 0x88
    f32 limit[3];      // 0x94
    f32 direction[3];  // 0xa0
    f32 phase[3];      // 0xac
};
static_assert(__builtin_offsetof(UnkOscillationShake_l767ba0, phase) == 0xac, "UnkOscillationShake_l767ba0 layout");

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * t;
}

static inline f32 Saturate(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

static inline CVector3D Cross(const CVector3D& a, const CVector3D& b) {
    CVector3D r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

static inline void AddScaled(CVector3D* pos, const CVector3D& v, f32 phase, f32 strength) {
    pos->x = pos->x + strength * (v.x * phase);
    pos->y = pos->y + strength * (v.y * phase);
    pos->z = pos->z + strength * (v.z * phase);
}

void fn_7100767ba0(UnkOscillationShake_l767ba0* self, void* camera, CVector3D* pos, const CVector3D* dir, void* a4,
                   void* a5, f32 dt) {
    if (!self->stopped && (self->duration <= 0.0f || self->time <= self->duration)) {
        f32 scale;
        UnkOscillationCurve_l767ba0* curve = self->curve;
        if (curve == reinterpret_cast<UnkOscillationCurve_l767ba0*>(&g_unk_7101d47108->empty)) {
            scale = 1.0f;
        } else {
            f32 t = self->time / self->period;
            ++curve->use_count;
            const char* text = self->curve->text;
            scale = fn_7100076020(Saturate(t), text ? text : g_unk_710162e2ed);
        }
        f32 strength = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                                     reinterpret_cast<const UnkShakePos_ovb0108*>(pos), scale);

        for (int i = 0; i < 3; ++i) {
            f32 step = self->speed[i] * dt;
            f32 direction = self->direction[i];
            f32 phase = self->phase[i] + step * direction;
            self->phase[i] = phase;
            if ((direction < 0.0f && phase <= -self->limit[i]) || (direction > 0.0f && phase >= self->limit[i])) {
                self->direction[i] = -direction;
                f32 a = self->amplitude[i];
                self->limit[i] = RandRange(a - a * 0.5f, a + a * 0.5f);
            }
        }

        CVector3D up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3D side = Cross(*dir, up);
        CVector3D lift = Cross(side, *dir);
        AddScaled(pos, side, self->phase[0], strength);
        AddScaled(pos, lift, self->phase[1], strength);
        AddScaled(pos, *dir, self->phase[2], strength);
    }
    fn_71010f3eb0(self, camera, pos, dir, a4, a5, dt);
}

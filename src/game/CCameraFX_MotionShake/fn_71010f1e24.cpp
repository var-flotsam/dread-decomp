// CCameraFX_MotionShake's unit (tools/tu_map.py unit 2698): unless paused, moves the camera position
// sideways, up and forward along its direction by the amounts at +0x18/+0x1c/+0x20, times the
// amplitude (+0x58), the curve's value over the elapsed time and the shake's falloff (fn_71010f3f20);
// then runs the base step. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVectorOps.h"
#include "game/CCameraFX_Shake.h"
#include "game/UnkTU53.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;

struct UnkMotionShake_l0f1e24 {
    u8 pad_0x0[0x18];
    f32 side;     // 0x18
    f32 up;       // 0x1c
    f32 forward;  // 0x20
    u8 pad_0x24[0x10];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 time;       // 0x50
    bool paused;    // 0x54
    u8 pad_0x55[0x3];
    f32 amplitude;  // 0x58
    u8 pad_0x5c[0x4];
    CStrId curve;  // 0x60
};
static_assert(__builtin_offsetof(UnkMotionShake_l0f1e24, curve) == 0x60, "");

static inline f32 Saturate(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010f1e24(UnkMotionShake_l0f1e24* self, void* camera, CVector3D* pos, const CVector3D* dir, void* arg4,
                   void* arg5, f32 dt) {
    if (!self->paused) {
        f32 strength = 1.0f;
        if (self->curve.data != &g_unk_7101d47108->empty)
            strength = fn_7100076020(Saturate(self->time / self->duration), self->curve.Text());
        f32 scale = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                                  reinterpret_cast<const UnkShakePos_ovb0108*>(pos), strength);
        if (self->amplitude > 0.0f) {
            CVector3D side = Cross(*dir, CVector3D{0.0f, 1.0f, 0.0f});
            CVector3D up = Cross(side, *dir);
            *pos = *pos + side * self->side * self->amplitude * scale;
            *pos = *pos + up * self->up * self->amplitude * scale;
            *pos = *pos + *dir * self->forward * self->amplitude * scale;
        }
    }
    fn_71010f3eb0(self, camera, pos, pos, arg4, arg5, dt);
}

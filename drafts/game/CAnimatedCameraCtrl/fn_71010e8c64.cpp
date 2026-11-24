// CAnimatedCameraCtrl's unit: the per-frame step of the animated camera. It advances the clock by the
// animation track's speed (negated when the actor plays backwards, held while the timer runs), keeps it
// inside the duration, and while idle starts the blend once the placement is close to its target; it ends
// the camera when the clock has run out, then runs the base step. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimatedCameraEnd_b0101;
void fn_71010e8ff8(UnkAnimatedCameraEnd_b0101* self);

struct UnkCamItem_l0e8c64 {
    u8 pad_0x0[0xdc];
    f32 duration;  // 0xdc
};

struct UnkCamAnim_l0e8c64 {
    u8 pad_0x0[0x4b8];
    u8 reversed;  // 0x4b8
};

struct UnkCamSpeed_l0e8c64 {
    u8 pad_0x0[0x18];
    f32 speed;  // 0x18
};

struct UnkCamActor_l0e8c64 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCamTarget_l0e8c64 {
    u8 pad_0x0[0x10];
    UnkCamActor_l0e8c64* actor;  // 0x10
};

struct UnkCamEntry_l0e8c64 {
    u8 pad_0x0[0x28];
    f32 length;  // 0x28
    u8 pad_0x2c[0xc];
    CStrId id;  // 0x38
};

using UnkCamPos_l0e8c64 = base::math::CVector3D;

struct UnkAnimatedCameraCtrl_l0e8c64 {
    u8 pad_0x0[0x38];
    UnkCamPos_l0e8c64 from_pos;  // 0x38
    UnkCamPos_l0e8c64 from_dir;  // 0x44
    u8 pad_0x50[0xc];
    f32 field_0x5c;
    f32 field_0x60;
    u8 pad_0x64[4];
    UnkCamPos_l0e8c64 to_pos;  // 0x68
    UnkCamPos_l0e8c64 to_dir;  // 0x74
    u8 pad_0x80[0xc];
    f32 field_0x8c;
    f32 field_0x90;
    u8 pad_0x94[8];
    u32 field_0x9c;
    u8 pad_0xa0[4];
    u32 field_0xa4;
    u8 pad_0xa8[0x1c];
    f32 field_0xc4;
    u8 pad_0xc8[0x1e0];
    UnkCamItem_l0e8c64* field_0x2a8;
    u8 pad_0x2b0[8];
    CStrId field_0x2b8;
    u8 field_0x2c0;
    u8 pad_0x2c1[3];
    f32 field_0x2c4;
    f32 field_0x2c8;
    u8 pad_0x2cc[0x1d0];
    f32 field_0x49c;
    f32 field_0x4a0;
    u8 pad_0x4a4[0x14];
    UnkCamTarget_l0e8c64* target;  // 0x4b8
    u8 pad_0x4c0[0x1c];
    s32 state;  // 0x4dc
    u8 pad_0x4e0[0xb9];
    u8 field_0x599;
    u8 field_0x59a;
    u8 pad_0x59b[0xd];
    u8 field_0x5a8[8];
};
static_assert(__builtin_offsetof(UnkAnimatedCameraCtrl_l0e8c64, field_0x5a8) == 0x5a8, "layout");

void fn_71010e8c64(UnkAnimatedCameraCtrl_l0e8c64* self, f32 dt) {
    f32 duration = self->field_0x2a8 ? self->field_0x2a8->duration : 0.0f;
    f32 limit = self->field_0x4a0 != -1.0f && self->field_0x4a0 < duration ? self->field_0x4a0 : duration;
    if (!self->field_0x2c0) {
        f32 speed = 1.0f;
        bool has_anim = false;
        UnkAnimationComponent* animation = nullptr;
        if (self->target && self->target->actor && self->target->actor->animation) {
            animation = self->target->actor->animation;
            UnkTrackQuery query = {fn_7100ceb8ec(), nullptr, -1.0f};
            fn_710117b08c(animation, &query);
            if (query.result)
                speed = reinterpret_cast<UnkCamSpeed_l0e8c64*>(query.result)->speed;
            has_anim = true;
        }
        bool paused = fn_710002fb70(self->field_0x5a8);
        f32 delta = paused ? 0.0f
                           : (has_anim && reinterpret_cast<UnkCamAnim_l0e8c64*>(animation)->reversed ? -(speed * dt)
                                                                                                      : speed * dt);
        self->field_0x2c8 = delta + self->field_0x2c8;
        bool advance = true;
        if (has_anim && self->field_0x59a) {
            const u64 key = 0xf4061ff25a2163d6;
            if (UnkCamEntry_l0e8c64* entry = static_cast<UnkCamEntry_l0e8c64*>(fn_7101177d2c(animation, key))) {
                advance = false;
                if (self->field_0x2b8.data == entry->id.data) {
                    f32 length = entry->length;
                    f32 at = limit * (fn_710117808c(animation, key) / length);
                    if (at > self->field_0x2c4)
                        self->field_0x2c4 = at - limit;
                } else if (self->field_0x2b8.data == &g_unk_7101d47108->empty) {
                    self->field_0x2b8 = entry->id;
                } else {
                    fn_71010e8ff8(reinterpret_cast<UnkAnimatedCameraEnd_b0101*>(self));
                }
            }
        }
        if (advance) {
            f32 t = delta + self->field_0x2c4;
            self->field_0x2c4 = t;
            if (t >= limit && self->field_0x599)
                self->field_0x2c4 = t - limit;
        }
    }
    s32 state = self->state;
    if (state == 0) {
        f32 blend = self->field_0x2c8 * self->field_0x49c;
        self->field_0xc4 = blend > 0.0f ? blend : 1.0f;
        f32 from_fov = self->field_0x5c;
        f32 from_roll = self->field_0x60;
        f32 to_fov = self->field_0x8c;
        f32 to_roll = self->field_0x90;
        f32 distance = base::math::Distance(self->from_pos, self->to_pos);
        f32 turn = fabsf(from_fov - to_fov);
        f32 close = base::math::Distance(self->from_dir, self->to_dir);
        if (turn < 1.0f && fabsf(from_roll - to_roll) < 1.0f && distance < 20.0f && close < 0.2f) {
            self->state = 1;
            self->field_0xa4 = 0;
            self->field_0xc4 = 0.0f;
            self->field_0x9c = 2;
        }
    }
    if (state == 0 || state == 1) {
        if (!self->field_0x2c0 && (!self->field_0x2a8 || self->field_0x2c4 >= limit))
            fn_71010e8ff8(reinterpret_cast<UnkAnimatedCameraEnd_b0101*>(self));
    }
    fn_7100763f9c(reinterpret_cast<CSynchronizedBlockCameraCtrl*>(self), dt);
}

// UnkTU437 (tools/tu_map.py unit 437, the CAimComponent unit): per-frame aim mode and angle. Takes the aim mode
// from fn_710034e490, raises the turn limit at +0x4d4 for the active conditions, then turns the angle at +0x340
// towards its target (+0x33c, or a CCharClassForceAnalogInputAngTrack's) by the tunable's speed times `value`
// and wraps it into [0, 2pi). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CGunComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkAimUpdate_b49;
struct UnkSamusGunComponent_b50;

struct UnkAimPlayer_l34a7b4 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
    u8 pad_0x3a9[0x7e4 - 0x3a9];
    bool field_0x7e4;
};

struct UnkAimOwner_l34a7b4 {
    u8 pad_0x0[0x208];
    UnkAimPlayer_l34a7b4* player;  // 0x208
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkSamusGunComponent_b50* gun;  // 0x2a8
};

struct UnkAimTunable_l34a7b4 {
    u8 pad_0x0[0x7c];
    f32 turn_speed;  // 0x7c
    u8 pad_0x80[0x88 - 0x80];
    f32 limit_blend;     // 0x88
    f32 limit_gun;       // 0x8c
    f32 limit_player;    // 0x90
};

struct UnkAimState_l34a7b4 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

// The record a CCharClassForceAnalogInputAngTrack query resolves to.
struct UnkForcedAngle_l34a7b4 {
    u8 pad_0x0[0x18];
    bool apply;      // 0x18
    f32 angle;       // 0x1c
    bool only_idle;  // 0x20
};

struct UnkAimComponent_l34a7b4 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l34a7b4* owner;  // 0x30
    u8 pad_0x38[0x50 - 0x38];
    UnkAimState_l34a7b4* state;  // 0x50
    u8 pad_0x58[0x2b0 - 0x58];
    UnkAimTunable_l34a7b4* tunable;  // 0x2b0
    u8 pad_0x2b8[0x33c - 0x2b8];
    f32 target_angle;  // 0x33c
    f32 angle;         // 0x340
    u8 pad_0x344[0x358 - 0x344];
    s32 mode;  // 0x358
    u8 pad_0x35c[0x4d0 - 0x35c];
    u32 field_0x4d0;
    f32 limit;  // 0x4d4
    u8 pad_0x4d8[0x969 - 0x4d8];
    bool field_0x969;
};

struct UnkAimAnimation_l34a7b4 {
    void** vtable;
    void vf_0x168(f32 value) { reinterpret_cast<void (*)(UnkAimAnimation_l34a7b4*, f32)>(vtable[0x168 / 8])(this, value); }
};

s32 fn_710034e490(UnkAimComponent_l34a7b4* self, bool* blend, bool* reset);

static inline f32 Max(f32 a, f32 b) {
    return a > b ? a : b;
}

void fn_710034a7b4(UnkAimUpdate_b49* aim_update, f32 value) {
    auto* self = reinterpret_cast<UnkAimComponent_l34a7b4*>(aim_update);
    if (!self->state || self->state->kind != 4) {
        UnkAimPlayer_l34a7b4* player = self->owner->player;
        s32 mode;
        if (player) {
            bool blend = false;
            bool reset = false;
            mode = fn_710034e490(self, &blend, &reset);
            if (reset)
                self->field_0x4d0 = 0;
            if (blend) {
                UnkAnimationComponent* animation = self->owner->animation;
                if (animation && fn_710117b348(animation, fn_7100d038f8()))
                    self->limit = Max(self->tunable->limit_blend, self->limit);
            }
            UnkSamusGunComponent_b50* gun = self->owner->gun;
            if (gun && fn_710084088c(gun))
                self->limit = Max(self->tunable->limit_gun, self->limit);
            if (player->field_0x7e4)
                self->limit = Max(self->tunable->limit_player, self->limit);
        } else {
            mode = 0;
        }
        if (self->mode != mode) {
            self->mode = mode;
            if (mode != 5)
                self->field_0x969 = false;
        }
        if (mode == 5 && player->field_0x3a8 && !fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(player)) &&
            !self->field_0x969)
            self->target_angle = 0.0f;
    }

    f32 target = self->target_angle;
    UnkTrackQuery query = {fn_7100c1c778(), nullptr, -1.0f};
    fn_710117b08c(self->owner->animation, &query);
    auto* forced = reinterpret_cast<UnkForcedAngle_l34a7b4*>(query.result);
    if (forced && (value >= 0.0f || !forced->only_idle)) {
        target = forced->angle;
        if (forced->apply)
            self->angle = target;
    }

    fn_710007a170(reinterpret_cast<UnkAim*>(&self->angle), target);
    if (target > self->angle) {
        if (value >= 0.0f) {
            self->angle += self->tunable->turn_speed * value;
            if (self->angle > target)
                self->angle = target;
        } else {
            self->angle = target;
        }
    } else if (target < self->angle) {
        if (value >= 0.0f) {
            self->angle -= self->tunable->turn_speed * value;
            if (self->angle < target)
                self->angle = target;
        } else {
            self->angle = target;
        }
    }
    f32 angle = fn_710007a1d0(self->angle);
    self->angle = angle < 0.0f ? angle + 6.2831855f : angle;

    if (UnkAnimationComponent* animation = self->owner->animation)
        reinterpret_cast<UnkAimAnimation_l34a7b4*>(animation)->vf_0x168(0.0f);
}

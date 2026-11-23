// CUsableComponent unit (tools/tu_map.py, unit 1607): the per-frame update. While interpolating, eases the
// player from the recorded start toward the owner's position (a sine ease over vf_0x2f0 seconds); then, unless
// the user's animation is playing one of the two usable animations, requests state 1 of the state machine at
// +0x100. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkUsablePos_laa2cf4 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkUsablePlayer_laa2cf4 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0x54 - 0x48];
    f32 y;
    u8 pad_0x58[0x64 - 0x58];
    f32 z;
    u8 pad_0x68[0x220 - 0x68];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x3b0 - 0x228];
    bool moved;
};

struct UnkUsableOwner_laa2cf4 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0x64 - 0x48];
    f32 z;
};

struct UnkUsableEntity_laa2cf4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkUsableUserHandle_laa2cf4 {
    u8 pad_0x0[0x10];
    UnkUsableEntity_laa2cf4* entity;
};

struct UnkUsableAnim_laa2cf4 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkUsable_laa2cf4 {
    virtual void vf_0x0();
    UnkUsableAnim_laa2cf4** vf_0x1c0() {
        typedef UnkUsableAnim_laa2cf4** (*Fn)(UnkUsable_laa2cf4*);
        return (*reinterpret_cast<Fn**>(this))[0x1c0 / 8](this);
    }
    UnkUsableAnim_laa2cf4** vf_0x1c8() {
        typedef UnkUsableAnim_laa2cf4** (*Fn)(UnkUsable_laa2cf4*);
        return (*reinterpret_cast<Fn**>(this))[0x1c8 / 8](this);
    }
    f32 vf_0x2f0() {
        typedef f32 (*Fn)(UnkUsable_laa2cf4*);
        return (*reinterpret_cast<Fn**>(this))[0x2f0 / 8](this);
    }
    u8 pad_0x8[0x30 - 0x8];
    UnkUsableOwner_laa2cf4* owner;
    u8 pad_0x38[0xc8 - 0x38];
    bool interpolating;
    u8 pad_0xc9[0xd0 - 0xc9];
    f32 elapsed;
    UnkUsablePos_laa2cf4 start;
    u8 pad_0xe0[0xe8 - 0xe0];
    UnkUsableUserHandle_laa2cf4* user;
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;
};
static_assert(__builtin_offsetof(UnkUsable_laa2cf4, machine) == 0x100, "layout");

// Taken by reference: the flag is loaded ahead of the player test, as in the original.
static inline bool Interpolating(UnkUsable_laa2cf4& self, UnkUsablePlayer_laa2cf4* player) {
    return player && self.interpolating;
}

static inline UnkUsablePos_laa2cf4 Lerp(const UnkUsablePos_laa2cf4& from, const UnkUsablePos_laa2cf4& to, f32 t) {
    f32 s = 1.0f - t;
    UnkUsablePos_laa2cf4 out = {t * to.x + s * from.x, t * to.y + s * from.y, t * to.z + s * from.z};
    return out;
}

void fn_7100aa2cf4(UnkUsable_laa2cf4* self, f32 dt) {
    auto* player = reinterpret_cast<UnkUsablePlayer_laa2cf4*>(fn_7100dec788(g_unk_7101d47310));
    if (Interpolating(*self, player)) {
        UnkAnimationComponent* animation = player->animation;
        if (animation && !fn_710117b348(animation, fn_7100c36de8())) {
            self->elapsed += dt;
            f32 duration = self->vf_0x2f0();
            f32 t = 1.0f;
            if (duration > 0.0f) {
                t = self->elapsed / duration;
                if (t < 1.0f)
                    t = (sinf((t * 2.0f - 1.0f) * 1.5707964f) + 1.0f) * 0.5f;
            }
            // The owner's position at the start's height.
            UnkUsablePos_laa2cf4 target = self->start;
            target.x = self->owner->x;
            target.z = self->owner->z;
            UnkUsablePos_laa2cf4 pos = !(t >= 1.0f) ? Lerp(self->start, target, t) : target;
            player->x = pos.x;
            player->y = pos.y;
            player->z = pos.z;
            player->moved = true;
            if (t >= 1.0f)
                self->interpolating = false;
        }
    }
    if (UnkAnimationComponent* animation = self->user->entity->animation) {
        if (fn_710117ae38(animation, (*self->vf_0x1c0())->id, false))
            return;
        if (fn_710117ae38(animation, (*self->vf_0x1c8())->id, false))
            return;
    }
    RequestState_A5_b73<15, 8>(&self->machine, 1);
}

// CBigFistAIComponent (unit 592): the per-frame update. After the base update (skipped while the game
// manager says so), the "patrol to combat" flag is dropped once the target stands in front, out of
// reach and the patrol-to-combat animation is not playing; the state is reset when the target's
// grab is about to end. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkBigFistGrabState_l3e79c8 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkBigFistGrab_l3e79c8 {
    u8 pad_0x0[0x50];
    UnkBigFistGrabState_l3e79c8* state;  // 0x50
    u8 pad_0x58[0x688 - 0x58];
    f32 start;  // 0x688
};

struct UnkBigFistTimer_l3e79c8 {
    u8 pad_0x0[0x184];
    f32 time;  // 0x184
};

struct UnkBigFistVec_l3e79c8 {
    f32 x, y, z;
};

inline UnkBigFistVec_l3e79c8 operator-(const UnkBigFistVec_l3e79c8& a, const UnkBigFistVec_l3e79c8& b) {
    UnkBigFistVec_l3e79c8 r = {a.x - b.x, a.y - b.y, a.z - b.z};
    return r;
}

inline f32 Dot(const UnkBigFistVec_l3e79c8& a, const UnkBigFistVec_l3e79c8& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct UnkBigFistActor_l3e79c8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38: fn_710002614c gives its matrix
    u8 pad_0x78[0x218 - 0x78];
    void* movement;  // 0x218
    UnkTakumakuComponent* animation;  // 0x220
    u8 pad_0x228[0x258 - 0x228];
    void* attack;  // 0x258
    u8 pad_0x260[0x2a8 - 0x260];
    UnkBigFistTimer_l3e79c8* timer;  // 0x2a8
    UnkBigFistGrab_l3e79c8* grab;    // 0x2b0

    f32 X() const { return transform.m[3]; }
    UnkBigFistVec_l3e79c8 Pos() const {
        UnkBigFistVec_l3e79c8 r = {transform.m[3], transform.m[7], transform.m[11]};
        return r;
    }
};
static_assert(__builtin_offsetof(UnkBigFistActor_l3e79c8, grab) == 0x2b0, "layout");

// The BigFist's own state (from +0x146ac of the component).
struct UnkBigFistState_l3e79c8 {
    u32 state;  // 0x0
    u8 pad_0x4[0x37c - 0x4];
    bool patrol_to_combat;  // 0x37c
    u8 pad_0x37d[0x4a4 - 0x37d];
    f32 grab_margin;  // 0x4a4
    bool watch_grab;  // 0x4a8
};

struct UnkBigFistAI_l3e79c8 {
    u8 pad_0x0[0x30];
    UnkBigFistActor_l3e79c8* owner;  // 0x30
    u8 pad_0x38[7];
    bool can_engage;  // 0x3f
    u8 pad_0x40[0x146ac - 0x40];
    UnkBigFistState_l3e79c8 fist;  // 0x146ac
};
static_assert(__builtin_offsetof(UnkBigFistAI_l3e79c8, fist) == 0x146ac, "layout");

void fn_71003e5ea0(UnkBigFistAI_l3e79c8* self, f32 dt);

void fn_71003e79c8(UnkBigFistAI_l3e79c8* self, f32 dt) {
    UnkBigFistState_l3e79c8& fist = self->fist;
    if (!fn_7100deabe4(g_unk_7101d47310))
        fn_71003e5ea0(self, dt);

    UnkBigFistActor_l3e79c8* target = reinterpret_cast<UnkBigFistActor_l3e79c8*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    if (target && fist.patrol_to_combat && self->owner->attack && !fn_7101136308(self->owner->attack)) {
        if (!self->can_engage) {
            fist.patrol_to_combat = false;
        } else if (self->owner->movement && fn_71010cab0c(self->owner->movement)) {
            const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
            UnkBigFistVec_l3e79c8 forward = {m.m[2], m.m[6], m.m[10]};
            if (Dot(forward, target->Pos() - self->owner->Pos()) >= 0.0f) {
                static CStrId s_anim("patrol_to_combat", true);
                if (fn_7101178e6c(self->owner->animation, 11) && self->owner->animation &&
                    !fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(self->owner->animation),
                                   reinterpret_cast<UnkCls_710117af10**>(&s_anim), false) &&
                    self->owner->movement && !fn_71010cab0c(self->owner->movement) &&
                    fabsf(self->owner->X() - target->X()) >= 450.0f)
                    fist.patrol_to_combat = false;
            }
        }
    }

    if (!target)
        return;
    UnkBigFistTimer_l3e79c8* timer = target->timer;
    UnkBigFistGrab_l3e79c8* grab = target->grab;
    if (timer && grab && fist.watch_grab && grab->state && grab->state->kind == 3) {
        if (timer->time - grab->start < fist.grab_margin + dt)
            fist.state = 0;
    }
}

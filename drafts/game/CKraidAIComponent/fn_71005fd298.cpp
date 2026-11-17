// The CKraidAIComponent unit: Kraid's per-frame update. Runs the boss update and the mouth's, keeps two timers,
// tracks whether its target is in reach (the flag at +0x14a14), measures the target's height against Kraid's
// head, steps the morph-ball launcher track and, when the target newly comes within reach, sends the owner's
// sub-object at +0x220 a request. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBossAI_l3ae15c;
struct UnkKraidMouthAI_b63a4;
struct UnkKraidAI3_b57a4;
struct UnkMagmaKraidController_b47;
struct UnkHelper;

void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);
void fn_71005fd5cc(UnkKraidMouthAI_b63a4* self, f32 dt);
void fn_71005fd9cc(UnkKraidAI3_b57a4* self);
bool fn_7100917c5c(UnkMagmaKraidController_b47* self);

// The target as this function sees it: the object at +0x1f0 whose flag at +0xf9 it reads.
struct UnkKraidTargetState_l5fd298 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkKraidTarget_l5fd298 {
    u8 pad_0x0[0x1f0];
    UnkKraidTargetState_l5fd298* field_0x1f0;
};

static inline UnkKraidTargetState_l5fd298* StateOf(UnkActor* target) {
    return reinterpret_cast<UnkKraidTarget_l5fd298*>(target)->field_0x1f0;
}

struct UnkKraidAI_l5fd298;
UnkVec3f fn_71005fd8c0(UnkKraidAI_l5fd298* self);  // a point of Kraid's head

struct UnkKraidAI_l5fd298 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149a8 - 0x38];
    f32 field_0x149a8;
    u8 pad_0x149ac[0x4];
    f32 field_0x149b0;
    u8 pad_0x149b4[0x4];
    UnkMagmaKraidController_b47* field_0x149b8;
    bool field_0x149c0;
    u8 pad_0x149c1[0x3];
    f32 field_0x149c4;
    u8 pad_0x149c8[0x8];
    f32 field_0x149d0;
    bool field_0x149d4;
    u8 pad_0x149d5[0x149e1 - 0x149d5];
    bool field_0x149e1;
    u8 pad_0x149e2[0x14a14 - 0x149e2];
    bool target_in_reach;  // 0x14a14
    u8 pad_0x14a15[0x14e00 - 0x14a15];
    bool field_0x14e00;

    // vtable slot 0x838
    void vf_0x838(f32 dt) {
        reinterpret_cast<void (*)(UnkKraidAI_l5fd298*, f32)>(vtable[0x838 / 8])(this, dt);
    }

};
// Refreshes whether the owner's target is in reach: it is while the target's helper says so, and stays so
// only while the target's state flag holds.
static inline bool RefreshInReach(UnkActor* owner, bool* in_reach) {
    UnkActor* target = fn_7100d7c990(owner);
    if (!target)
        *in_reach = false;
    else if (target->field_0x208 && fn_71009e6434(static_cast<UnkHelper*>(target->field_0x208)))
        *in_reach = true;
    else
        *in_reach = *in_reach && StateOf(target)->field_0xf9;
    return *in_reach;
}

static_assert(__builtin_offsetof(UnkKraidAI_l5fd298, field_0x149e1) == 0x149e1, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fd298, target_in_reach) == 0x14a14, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fd298, field_0x14e00) == 0x14e00, "layout");

void fn_71005fd298(UnkKraidAI_l5fd298* self, f32 dt) {
    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
    if (self->field_0x149a8 >= 0.0f)
        self->field_0x149a8 += dt;
    fn_71005fd5cc(reinterpret_cast<UnkKraidMouthAI_b63a4*>(self), dt);
    if (self->field_0x149c4 > 0.0f) {
        self->field_0x149c4 -= dt;
        if (self->field_0x149c4 < 0.0f)
            self->field_0x149c4 = 0.0f;
    }

    if (!RefreshInReach(self->owner, &self->target_in_reach)) {
        self->field_0x149d4 = false;
        f32 top = fn_7100d7c990(self->owner)->field_0x54 + 200.0f;
        self->field_0x149e1 = top < fn_71005fd8c0(self).y;
    }

    if (self->field_0x149b0 == 0.0f) {
        if (self->field_0x149c0)
            self->field_0x149d0 += dt;
        else
            fn_71005fd9cc(reinterpret_cast<UnkKraidAI3_b57a4*>(self));
    }

    auto* track = static_cast<UnkAnimationComponent*>(fn_7100d7c990(self->owner)->field_0x220);
    if (track && fn_710117b348(track, fn_7100c4041c()))
        self->vf_0x838(dt);

    bool grabbing;
    if (RefreshInReach(self->owner, &self->target_in_reach) && fn_7100917c5c(self->field_0x149b8))
        grabbing = true;
    else
        grabbing = self->field_0x14e00 && StateOf(fn_7100d7c990(self->owner))->field_0xf9;
    if (!self->field_0x14e00 && grabbing)
        UnkSpitSend(self->owner->field_0x220, 0x6afc278b490b3d0a, 0xf4061ff25a2163d6);
    self->field_0x14e00 = grabbing;
}

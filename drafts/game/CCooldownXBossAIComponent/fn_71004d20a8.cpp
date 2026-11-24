// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): the update (fn_71004d20a8). After the boss
// update it leaves the grab state when the flag at +0x15836 asks, then in state 1 either steps the attack
// object's value by the current phase's tunable rate or moves the target position; it counts the timer at
// +0x15684 down and runs the per-frame checks. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAttackComponent.h"
#include "game/CGrabComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkBossAI_l3ae15c;
struct UnkCooldownXMsgAI_ovb0088;
struct UnkCooldownXBoundsAI_ovb0088;
struct UnkMover;

extern base::reflection::UnkTunable g_unk_71726d8260;

// The Cooldown X boss's tunables as the update reads them: a value range per phase and the time to cross it.
struct UnkCooldownXRange_l4d20a8 {
    f32 low;
    f32 high;
};

struct UnkCooldownXTunable_l4d20a8 {
    u8 pad_0x0[0x30];
    UnkCooldownXRange_l4d20a8 phase0;  // 0x30
    UnkCooldownXRange_l4d20a8 phase1;  // 0x38
    UnkCooldownXRange_l4d20a8 phase2;  // 0x40
    UnkCooldownXRange_l4d20a8 phase3;  // 0x48
    f32 duration;                      // 0x50
};

struct UnkCooldownXValueObject_l4d20a8 {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 value;  // 0x54

    void SetValue(f32 v, bool flag) {
        reinterpret_cast<void (*)(UnkCooldownXValueObject_l4d20a8*, f32, bool)>(vtable[0x168 / 8])(this, v, flag);
    }
};

struct UnkCooldownXOwner_l4d20a8 {
    u8 pad_0x0[0x1b8];
    UnkCooldownXValueObject_l4d20a8* value_object;  // 0x1b8
    u8 pad_0x1c0[0x98];
    void* attack;  // 0x258
    void* grab;    // 0x260
};

struct UnkCooldownXAI_l4d20a8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkCooldownXOwner_l4d20a8* owner;  // 0x30
    u8 pad_0x38[0x1108];
    s32 state;  // 0x1140
    u8 pad_0x1144[0x1453c];
    s32 phase;  // 0x15680
    f32 timer;  // 0x15684
    u8 pad_0x15688[0x198];
    CVector3D position;  // 0x15820
    u8 pad_0x1582c[0xa];
    bool leave_grab;  // 0x15836

    void SetState(s32 next, bool flag) {
        reinterpret_cast<void (*)(UnkCooldownXAI_l4d20a8*, s32, bool)>(vtable[0x208 / 8])(this, next, flag);
    }
};
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d20a8, timer) == 0x15684, "layout");
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d20a8, position) == 0x15820, "layout");
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d20a8, leave_grab) == 0x15836, "layout");

void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);
void fn_71004d2258(UnkCooldownXMsgAI_ovb0088* self, f32 value);
void fn_71004d24d0(UnkMover* self, CVector3D* position, f32 scale);
void fn_71004d262c(UnkCooldownXAI_l4d20a8* self, CVector3D* position, f32 dt);
void fn_71004d28cc(UnkCooldownXAI_l4d20a8* self);
void fn_71004d29dc(UnkCooldownXAI_l4d20a8* self);
void CCooldownXBossAIComponent_UpdateHeadCollidersByLocatorHeight(UnkCooldownXBoundsAI_ovb0088* self);

void fn_71004d20a8(UnkCooldownXAI_l4d20a8* self, f32 dt) {
    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
    if (self->leave_grab && !fn_710113d83c(self->owner->grab) && self->state != 1)
        self->SetState(1, true);
    if (self->state == 1) {
        if (fn_7101136308(self->owner->attack)) {
            fn_71004d2258(reinterpret_cast<UnkCooldownXMsgAI_ovb0088*>(self), dt);
            if (UnkCooldownXValueObject_l4d20a8* object = self->owner->value_object) {
                UnkCooldownXTunable_l4d20a8* tunable = reinterpret_cast<UnkCooldownXTunable_l4d20a8*>(&g_unk_71726d8260);
                f32 current = object->value;
                // The value moves across the phase's range in the tunable's duration.
                auto advance = [&](f32 low, f32 high) {
                    f32 next = current;
                    next += (high - low) / tunable->duration * dt;
                    return next;
                };
                f32 value;
                switch (self->phase) {
                case 0:
                    value = advance(tunable->phase0.low, tunable->phase0.high);
                    break;
                case 1:
                    value = advance(tunable->phase1.low, tunable->phase1.high);
                    break;
                case 2:
                    value = advance(tunable->phase2.low, tunable->phase2.high);
                    break;
                case 3:
                    value = advance(tunable->phase3.low, tunable->phase3.high);
                    break;
                default:
                    value = advance(0.0f, 0.0f);
                    break;
                }
                f32 max = tunable->phase0.high;
                object->SetValue(value < max ? value : max, true);
            }
        } else {
            fn_71004d24d0(reinterpret_cast<UnkMover*>(self), &self->position, dt);
            fn_71004d262c(self, &self->position, dt);
        }
    }
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer < 0.0f)
            self->timer = 0.0f;
    }
    fn_71004d28cc(self);
    fn_71004d29dc(self);
    CCooldownXBossAIComponent_UpdateHeadCollidersByLocatorHeight(reinterpret_cast<UnkCooldownXBoundsAI_ovb0088*>(self));
}

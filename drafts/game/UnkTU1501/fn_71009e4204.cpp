// UnkTU1501 (CSamusMovement's unit): when the setting at +0x7a allows it and nothing holds the actor (input,
// animation state sets, mask 4, a non-snapped rotation track), snaps a facing angle that is off +-90 degrees
// to the side the actor faces, or, while hanging, to the side the current hang or grab state faces.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CInputComponent.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAnimMaskHolder_b47a3;
struct UnkGrappleMoveState_B71a6;
bool CAnimationComponent_AnyPlayingTrackHasFlags(UnkAnimMaskHolder_b47a3* self, const u32* bits);
bool fn_71009ad430(UnkGrappleMoveState_B71a6* state);

struct UnkSettings_l9e4204 {
    u8 pad_0x0[0x7a];
    bool field_0x7a;
};

struct UnkMoveState_l9e4204 {
    u8 pad_0x0[0x10];
    u32 id;
    u8 pad_0x14[0x1a8 - 0x14];
    f32 field_0x1a8;
    u8 pad_0x1ac[0x1c4 - 0x1ac];
    f32 field_0x1c4;
};

struct UnkMoveMachine_l9e4204 {
    void** vtable;
    void* owner;
    UnkMoveState_l9e4204* current;
    UnkMoveState_l9e4204* previous;
    u32 count;
    u32 states[0x20];  // UnkMoveState_l9e4204* entries, 4-byte aligned
    UnkMoveState_l9e4204** States() { return reinterpret_cast<UnkMoveState_l9e4204**>(states); }

    UnkMoveState_l9e4204* Find(u32 id) {
        if (id >= 15)
            return nullptr;
        UnkMoveState_l9e4204** begin = States();
        UnkMoveState_l9e4204** end = begin + count;
        UnkMoveState_l9e4204** p = begin;
        for (; p != end; ++p)
            if ((*p)->id == id)
                break;
        return p != end ? *p : nullptr;
    }

    UnkMoveState_l9e4204* FindCurrent(u32 id) {
        UnkMoveState_l9e4204* state = Find(id);
        return state && state == current ? state : nullptr;
    }
};

struct UnkSamusMovement_l9e4204 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x338 - 0x38];
    UnkXform xform;
    u8 pad_0x394[0x2cf8 - 0x394];
    UnkMoveMachine_l9e4204 machine;
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9e4204, machine) == 0x2cf8, "layout");

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static const f32 kHalfPi = 1.5707964f;

void fn_71009e4204(UnkSamusMovement_l9e4204* self) {
    if (!reinterpret_cast<const UnkSettings_l9e4204&>(g_unk_7172740de0).field_0x7a)
        return;
    UnkActor* owner = self->owner;
    if (UnkInputComponent* input = static_cast<UnkInputComponent*>(owner->field_0x1d8)) {
        if (fn_710087238c(input))
            return;
    }
    if (fn_7101179870(static_cast<UnkStateSet_1441*>(self->owner->field_0x220)))
        return;
    UnkAnimMaskHolder_b47a3* masks = static_cast<UnkAnimMaskHolder_b47a3*>(self->owner->field_0x220);
    alignas(8) u32 bits = 4;
    if (CAnimationComponent_AnyPlayingTrackHasFlags(masks, &bits))
        return;

    UnkVec3f angle = self->xform.GetAngle();
    if (Near(angle.y, -kHalfPi) || Near(angle.y, kHalfPi))
        return;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100c412fc()))
        return;

    angle.y = Sign(angle.y) * kHalfPi;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (animation && fn_710117b348(animation, fn_7100d029c8())) {
        UnkMoveState_l9e4204* current = self->machine.current;
        if (current && current->id == 13) {
            UnkMoveState_l9e4204* state = self->machine.FindCurrent(fn_71009a60fc());
            if (state && fn_71009ad430(reinterpret_cast<UnkGrappleMoveState_B71a6*>(state)))
                angle.y = Sign(-state->field_0x1c4) * kHalfPi;
        } else if (UnkMoveState_l9e4204* state = self->machine.FindCurrent(6)) {
            angle.y = Sign(state->field_0x1a8) * kHalfPi;
        }
    }
    self->xform.SetAngle(angle);
}

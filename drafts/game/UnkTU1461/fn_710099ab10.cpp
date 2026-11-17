// CSamusGrappleMovementState's unit (tools/tu_map.py, unit 1461): entering the grapple state with a kind.
// Runs the base and kind steps, clears the hold values, takes a weak handle to the grapple beam gun's
// grabbed target and requests the state the target reports from the state machine at +0x188 ("none" 5),
// then sets the owner's timeline key g_unk_710164b368. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CGunComponent.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkComponent;
struct CSamusMovementState2;
struct UnkGrappleState_B71a6;
void fn_71009b0c74(CSamusMovementState2* self);
void fn_710099adcc(UnkGrappleState_B71a6* self, s32 kind);
extern const u64 g_unk_710164b368;

struct UnkGrappleTarget_l99ab10 : UnkWeakTarget {
    s32 vf_0x178() {
        return reinterpret_cast<s32 (*)(UnkGrappleTarget_l99ab10*)>((*reinterpret_cast<void***>(this))[0x178 / 8])(
            this);
    }
};

struct UnkGrappleHold_l99ab10 {
    u8 pad_0x0[0x10];
    UnkGrappleTarget_l99ab10* target;
};
struct UnkGrappleBeam_l99ab10 {
    u8 pad_0x0[0x548];
    UnkGrappleHold_l99ab10* hold;
};
struct UnkGrappleBeamOwner_l99ab10 {
    u8 pad_0x0[0x10];
    UnkGrappleBeam_l99ab10* beam;
};
struct UnkGrappleBeamGun_l99ab10 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x1888 - 0x8];
    UnkGrappleBeamOwner_l99ab10* field_0x1888;
};

struct UnkGrappleActor_l99ab10 {
    u8 pad_0x0[0x2a8];
    UnkComponent* field_0x2a8;
};
struct UnkGrappleOwner_l99ab10 {
    u8 pad_0x0[0x30];
    UnkGrappleActor_l99ab10* actor;
};
struct UnkGrappleMovement_l99ab10 {
    u8 pad_0x0[0x8];
    UnkGrappleOwner_l99ab10* owner;
};

struct UnkGrappleHoldValues_l99ab10 {
    base::math::CVector2D field_0x0;
    base::math::CVector2D field_0x8;
};

struct UnkGrappleState_l99ab10 {
    u8 pad_0x0[0x8];
    UnkGrappleMovement_l99ab10* movement;
    u8 pad_0x10[0x188 - 0x10];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x238[0xb18 - 0x238];
    UnkWeakHandle* target;
    u8 pad_0xb20[0xb90 - 0xb20];
    bool field_0xb90;
    UnkGrappleHoldValues_l99ab10 hold;
};
static_assert(__builtin_offsetof(UnkGrappleState_l99ab10, hold) == 0xb94, "layout");

// The owner's grapple beam gun, or null.
static inline UnkGrappleBeamGun_l99ab10* GrappleGun(UnkGrappleState_l99ab10* self) {
    UnkComponent* c = self->movement->owner->actor->field_0x2a8;
    if (c == nullptr)
        return nullptr;
    auto* gun = static_cast<UnkGrappleBeamGun_l99ab10*>(fn_710083cd30(c));
    if (gun == nullptr)
        return nullptr;
    return fn_710009d50c(gun->vf_0x0(), fn_7100832034()) ? gun : nullptr;
}

void fn_710099ab10(UnkGrappleState_l99ab10* self, s32 kind) {
    fn_71009b0c74(reinterpret_cast<CSamusMovementState2*>(self));
    UnkGrappleActor_l99ab10* actor = self->movement->owner->actor;
    fn_710099adcc(reinterpret_cast<UnkGrappleState_B71a6*>(self), kind);
    self->field_0xb90 = false;
    self->hold = UnkGrappleHoldValues_l99ab10();
    if (UnkGrappleBeamGun_l99ab10* gun = GrappleGun(self)) {
        if (UnkGrappleHold_l99ab10* hold = gun->field_0x1888->beam->hold) {
            if (UnkGrappleTarget_l99ab10* target = hold->target) {
                UnkWeakHandle* handle = GetWeakHandle(target);
                AssignRef(&self->target, handle);
                s32 state = static_cast<UnkGrappleTarget_l99ab10*>(handle->target)->vf_0x178();
                if (state != 5)
                    RequestState_A5_b73<4, 5>(&self->machine, state);
            }
        }
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(actor), &g_unk_710164b368, false);
}

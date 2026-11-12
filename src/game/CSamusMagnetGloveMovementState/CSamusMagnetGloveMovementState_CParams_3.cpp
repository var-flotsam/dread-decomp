// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009ae954 is the twin of fn_71009ae8e8
// (CSamusMagnetGloveMovementState_CParams_2.cpp): false when the value at +0x190 is zero, else whether
// fn_7100976c94 holds for the actor's (+0x30) position against the target at +0x38. Names and
// layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrappleTarget;

struct UnkMagnetGloveTarget3 {
    u8 unk_0x0[0x3c8];
    u32 field_0x3c8;
};

struct UnkMagnetGloveState3 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    UnkMagnetGloveTarget3* target;
    u8 pad_0x40[0x150];
    f32 field_0x190;
};

bool fn_71009ae954(UnkMagnetGloveState3* self) {
    if (self->field_0x190 != 0.0f) {
        UnkMagnetGloveTarget3* target = self->target;
        UnkVec3f pos = reinterpret_cast<const UnkXform*>(self->actor->field_0x38)->GetTranslation();
        if (fn_7100976c94(reinterpret_cast<UnkGrappleTarget*>(target), &pos, target->field_0x3c8, nullptr, true, 0.0f))
            return true;
    }
    return false;
}

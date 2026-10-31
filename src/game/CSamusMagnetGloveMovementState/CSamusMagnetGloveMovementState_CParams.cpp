// In the CSamusMagnetGloveMovementState::CParams unit: true when the value at +0x190 is zero, else
// whether fn_7100976c94 fails for the actor's (+0x30) position against the target at +0x38 (as
// fn_710081b434 in src/game/CGrappleBeamComponent.cpp asks it). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrappleTarget;
bool fn_7100976c94(UnkGrappleTarget* target, const UnkVec3f* pos, u32 value, void* arg, bool flag, f32 offset);

struct UnkMagnetGloveTarget {
    u8 unk_0x0[0x3c8];
    u32 field_0x3c8;
};

struct UnkMagnetGloveState {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    UnkMagnetGloveTarget* target;
    u8 pad_0x40[0x150];
    f32 field_0x190;
};

bool fn_71009ae9c0(UnkMagnetGloveState* self) {
    if (self->field_0x190 != 0.0f) {
        UnkMagnetGloveTarget* target = self->target;
        UnkVec3f pos = reinterpret_cast<const UnkXform*>(self->actor->field_0x38)->GetTranslation();
        if (fn_7100976c94(reinterpret_cast<UnkGrappleTarget*>(target), &pos, target->field_0x3c8, nullptr, true, 0.0f))
            return false;
    }
    return true;
}

// behaviortree::CSetAnimDependsOnDirAction unit. fn_710036f800: the action's start; after the base start
// it decides whether the owner is busy (its CEnemyMovement in state 4, or turning on a CCharClassTurnTrack)
// and sets the done/waiting flags, else picks the animation (fn_710036f938). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkOwner_l36f800 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSelf_l36f800 {
    u8 pad_0x0[0x18];
    UnkOwner_l36f800* owner;  // 0x18
    u8 pad_0x20[0x68];
    u8 field_0x88;
    u8 pad_0x89;
    u8 checkMovement;  // 0x8a
    u8 pad_0x8b;
    u8 checkTurning;  // 0x8c
    u8 pad_0x8d[0x1e2 - 0x8d];
    u8 done;     // 0x1e2
    u8 waiting;  // 0x1e3
};

void fn_710036f938(UnkSelf_l36f800* self);

void fn_710036f800(UnkSelf_l36f800* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->field_0x88 = 0;
    UnkOwner_l36f800* owner = self->owner;
    UnkAnimationComponent* animation = owner ? owner->animation : nullptr;
    UnkReflObject* movement = owner->movement;
    bool isMovement = movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0());
    if (!isMovement)
        movement = nullptr;
    auto movementBusy = [&] { return isMovement && self->checkMovement && fn_71010cc3a4(movement) == 4; };

    if (movementBusy() ||
        (animation && self->checkTurning && fn_710117b348(animation, fn_7100d2b3a8()))) {
        if (movementBusy())
            self->done = 1;
        else
            self->waiting = 1;
    } else {
        self->done = 0;
        self->waiting = 0;
    }
    if (!self->done && !self->waiting)
        fn_710036f938(self);
}

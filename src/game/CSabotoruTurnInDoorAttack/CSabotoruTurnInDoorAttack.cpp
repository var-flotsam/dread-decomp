// CSabotoruTurnInDoorAttack: fn_7100673b60 runs the attack base step (fn_71003609b8), then, when the
// weak handle at +0x50 still has a target, hands the target the object at +0x38 (fn_710117b3f0) and
// drops the handle (as fn_71005c7210); finally tells the actor's AI component (+0x240), when it is
// a CSabotoruAIComponent, fn_7100679410 with 1. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSabotoruAIComponent.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSabotoruTurnInDoor : UnkAttackBase {
    u8 pad_0x20[0x18];
    u8 field_0x38[0x18];
    UnkWeakHandle* field_0x50;
};

void fn_7100673b60(UnkSabotoruTurnInDoor* self) {
    fn_71003609b8(self);
    if (UnkWeakHandle* handle = self->field_0x50) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x38);
        ResetRef(&self->field_0x50);
    }
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100677dd4()))
        fn_7100679410(ai, 1);
}

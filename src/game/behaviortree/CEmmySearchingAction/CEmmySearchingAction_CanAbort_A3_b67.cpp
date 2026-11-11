// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): whether the search can be left:
// false without the owner's movement (+0x218); true when the movement is in mode 4 (fn_71010cc3a4) and
// the character class has no CCharClassCanBeAbortedTrack, or when the E.M.M.I. AI's timer at +0x29418
// is running (fn_710002fb70) or its flag at +0x29f26 is set; else the movement's own answer
// (fn_71010cab0c). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

// As src/game/behaviortree/CInfesterFleeFromTargetAction/CInfesterFleeFromTargetAction.cpp declares it.
// As src/game/CDoorLifeComponent/CDoorLifeComponent_b44.cpp declares it.
// A refl_CallWithOwnerChild instantiation in src/clones/clones_06.cpp.

struct UnkEmmySearchingAction_A3_b67 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
};

bool fn_7100518494(UnkEmmySearchingAction_A3_b67* self) {
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    u8* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<u8*>(ai) : nullptr;
    void* movement = self->owner->field_0x218;
    if (!movement)
        return false;
    auto* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (fn_71010cc3a4(movement) == 4 && !fn_710117b348(anim, fn_7100c0f988()))
        return true;
    if (fn_710002fb70(emmy + 0x29418) || emmy[0x29f26])
        return true;
    return fn_71010cab0c(self->owner->field_0x218);
}

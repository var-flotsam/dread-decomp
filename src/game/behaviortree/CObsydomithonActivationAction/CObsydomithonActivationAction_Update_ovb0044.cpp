// behaviortree::CObsydomithonActivationAction's unit (tools/tu_map.py unit 946). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActivationAction_ovb0044.h"
#include "game/UnkActivationOwner_ovb0044.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_710062f260(UnkReflObject* ai);

// Updates the action: while the owner's AI component (+0x240), as a CObsydomithonAIComponent, says
// fn_710062f260 holds, the action runs on (5); otherwise it ends (2).
static inline bool IsActivating(UnkActivationAction_ovb0044* self) {
    UnkReflObject* ai = self->owner->ai;
    if (!ai)
        return false;
    return fn_710062f260(fn_710009d50c(ai->vf_0x0(), fn_710062c51c()) ? ai : nullptr);
}

u32 fn_710062b48c(UnkActivationAction_ovb0044* self) {
    return IsActivating(self) ? 5 : 2;
}

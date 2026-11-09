// behaviortree::CVulkranActivationAction's unit (tools/tu_map.py unit 1081). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkActivationOwner_ovb0044 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkActivationAction_ovb0044 {
    u8 pad_0x0[0x18];
    UnkActivationOwner_ovb0044* owner;  // 0x18
};

bool fn_71006dcdec(UnkReflObject* ai);

// Updates the action: while the owner's AI component (+0x240), as a CVulkranAIComponent, says
// fn_71006dcdec holds, the action runs on (5); otherwise it ends (2).
static inline bool IsActivating(UnkActivationAction_ovb0044* self) {
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71006db2b8()))
        return false;
    return fn_71006dcdec(ai);
}

u32 fn_71006d6f88(UnkActivationAction_ovb0044* self) {
    return IsActivating(self) ? 5 : 2;
}

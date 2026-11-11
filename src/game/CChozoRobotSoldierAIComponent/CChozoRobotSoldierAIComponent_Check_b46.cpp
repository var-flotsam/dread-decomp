// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py): true when the owner's (+0x30) target
// (fn_7100d7c990) has an object at +0x208 whose count (fn_71009e8590) is above 1; otherwise the
// base AI component's answer (fn_7101105dac). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "types.h"

// Signature as src/game/CChozoWarriorAIComponent/fn_710049cbd8.cpp declares it.
struct UnkFlagged;

struct UnkChozoRobotSoldierAI_b46a1 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

bool fn_7101105dac(UnkChozoRobotSoldierAI_b46a1* self);

bool fn_7100474c58(UnkChozoRobotSoldierAI_b46a1* self) {
    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        if (target->field_0x208 && fn_71009e8590(static_cast<UnkFlagged*>(target->field_0x208)) > 1)
            return true;
    }
    return fn_7101105dac(self);
}

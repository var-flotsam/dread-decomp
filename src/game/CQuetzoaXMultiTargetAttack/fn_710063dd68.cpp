// Copies of fn_71003e2ea8 (src/game/behaviortree/CBigFistChangeToCombatAction/fn_71003e2ea8.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_710063dd68.
// Destructor body: runs the base destructor (fn_7100394bd4), then, when the weak handle at +0x58 still has a
// target, hands the target the object at +0x40 (fn_710117b3f0) and drops the handle.
#include "game/UnkAttack.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf : UnkAttackBase {
    u8 pad_0x20[0x20];
    u8 field_0x40[0x18];
    UnkWeakHandle* field_0x58;
};

void fn_710063dd68(UnkSelf* self) {
    fn_71003609b8(self);
    if (UnkWeakHandle* handle = self->field_0x58) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x40);
        ResetRef(&self->field_0x58);
    }
}

// Destructor body: runs the base destructor (fn_7100394bd4), then, when the weak handle at +0x58 still has a
// target, hands the target the object at +0x40 (fn_710117b3f0) and drops the handle.
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x40];
    u8 field_0x40[0x18];
    UnkWeakHandle* field_0x58;
};

void fn_71003e3a50(UnkSelf* self) {
    fn_7100394bd4(self);
    if (UnkWeakHandle* handle = self->field_0x58) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x40);
        ResetRef(&self->field_0x58);
    }
}

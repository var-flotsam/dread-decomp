// Destructor body: runs the base destructor (fn_71004936a0), then, when the weak handle at +0x60 still has a
// target, hands the target the object at +0x48 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x48];
    u8 field_0x48[0x18];
    UnkWeakHandle* field_0x60;
};

void fn_71004936a0(UnkSelf* self);

void fn_7100480928(UnkSelf* self) {
    fn_71004936a0(self);
    if (UnkWeakHandle* handle = self->field_0x60) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x48);
        ResetRef(&self->field_0x60);
    }
}

// Destructor body: runs the base destructor (fn_7100494c98), then, when the weak handle at +0x68 still has a
// target, hands the target the object at +0x50 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x50];
    u8 field_0x50[0x18];
    UnkWeakHandle* field_0x68;
};

void fn_7100494c98(UnkSelf* self);

void fn_71004818b0(UnkSelf* self) {
    fn_7100494c98(self);
    if (UnkWeakHandle* handle = self->field_0x68) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x50);
        ResetRef(&self->field_0x68);
    }
}

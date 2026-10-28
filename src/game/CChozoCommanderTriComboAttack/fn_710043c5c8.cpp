// Destructor body: runs the base destructor (fn_710042bb40), then, when the weak handle at +0x78 still has a
// target, hands the target the object at +0x60 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x60];
    u8 field_0x60[0x18];
    UnkWeakHandle* field_0x78;
};

void fn_710042bb40(UnkSelf* self);

void fn_710043c5c8(UnkSelf* self) {
    fn_710042bb40(self);
    if (UnkWeakHandle* handle = self->field_0x78) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x60);
        ResetRef(&self->field_0x78);
    }
}

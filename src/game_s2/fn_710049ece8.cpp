// Destructor body: runs the base destructor (fn_71003609b8), then, when the weak handle at +0x50 still has a
// target, hands the target the object at +0x38 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x18];
    UnkWeakHandle* field_0x50;
};

void fn_71003609b8(UnkSelf* self);
void fn_710117b3f0(UnkWeakTarget* target, void* object);

void fn_710049ece8(UnkSelf* self) {
    fn_71003609b8(self);
    if (UnkWeakHandle* handle = self->field_0x50) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x38);
        ResetRef(&self->field_0x50);
    }
}

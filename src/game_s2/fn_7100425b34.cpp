// Destructor body: runs the base destructor (fn_7100424b58), then, when the weak handle at +0x80 still has a
// target, hands the target the object at +0x68 (fn_710117b3f0) and drops the handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x68];
    u8 field_0x68[0x18];
    UnkWeakHandle* field_0x80;
};

void fn_7100424b58(UnkSelf* self);
void fn_710117b3f0(UnkWeakTarget* target, void* object);

void fn_7100425b34(UnkSelf* self) {
    fn_7100424b58(self);
    if (UnkWeakHandle* handle = self->field_0x80) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x68);
        ResetRef(&self->field_0x80);
    }
}

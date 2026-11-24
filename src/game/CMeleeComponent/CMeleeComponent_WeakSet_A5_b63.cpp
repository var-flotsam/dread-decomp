// The CMeleeComponent unit: points the weak handle at +0x8 at `target` and marks the change (flag 0x400
// at +0x3c), or drops it when there is no target. Names and layout are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakTargetSlot_A5_b63 {
    u64 field_0x0;
    UnkWeakHandle* handle;  // 0x8
    u8 field_0x10[0x2c];
    u32 flags;  // 0x3c
};

void fn_710092f0b4(UnkWeakTargetSlot_A5_b63* self, UnkWeakTarget* target) {
    if (!target) {
        ResetRef(&self->handle);
        return;
    }
    AssignRef(&self->handle, GetWeakHandle(target));
    self->flags |= 0x400;
}

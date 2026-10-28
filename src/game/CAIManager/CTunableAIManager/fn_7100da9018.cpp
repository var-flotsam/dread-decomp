// Sets a timed weak reference (at 0x2410, time at 0x2418) to `target` when fn_71004067a4 accepts
// it, running fn_7100da9194 at once when the time is not positive; otherwise clears both.
#include "game_s2/UnkWeakHandle.h"

struct UnkTimedRef {
    u8 unk_0x0[0x2410];
    UnkWeakHandle* field_0x2410;
    f32 field_0x2418;
};

void* fn_71004067a4(void* owner);
void fn_7100da9194(UnkTimedRef* self);

// NON_MATCHING(minor): the target and the time are moved to their saved registers in the other
// order.
bool fn_7100da9018(UnkTimedRef* self, UnkWeakTarget* target, f32 time) {
    if (target && fn_71004067a4(target)) {
        AssignRef(&self->field_0x2410, GetWeakHandle(target));
        self->field_0x2418 = time;
        if (time <= 0.0f)
            fn_7100da9194(self);
        return true;
    }
    ResetRef(&self->field_0x2410);
    self->field_0x2418 = 0.0f;
    return false;
}

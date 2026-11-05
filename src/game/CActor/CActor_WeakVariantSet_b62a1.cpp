// The CActor unit: points a two-state value at the weak handle of `target` (state 2, the handle
// created on first use) or clears it (state 0) when there is no target. Names and layout are
// placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakVariant_b62a1 {
    u8 field_0x0[0x30];
    u32 state;  // 0x30
    UnkWeakHandle* handle;  // 0x38
};

void fn_710023df08(UnkWeakVariant_b62a1* self, UnkWeakTarget* target) {
    if (target) {
        AssignRef(&self->handle, GetWeakHandle(target));
        self->state = 2;
    } else {
        ResetRef(&self->handle);
        self->state = 0;
    }
}

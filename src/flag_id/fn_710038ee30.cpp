// Countdown timer tick. Returns 6 when disabled, else 5 or 2 depending on a flag.
#include "types.h"

struct UnkTimer {
    u8 unk_0x0[0x3c];
    f32 interval;
    f32 remaining;
    u8 unk_0x44;
    bool flag_0x45;
    u8 unk_0x46[0xa];
    bool enabled;
};

void fn_710038eae4(UnkTimer* self);

s32 fn_710038ee30(UnkTimer* self, f32 dt) {
    if (!self->enabled)
        return 6;
    if (self->interval > 0.0f) {
        self->remaining -= dt;
        if (self->remaining <= 0.0f) {
            self->remaining = self->interval;
            fn_710038eae4(self);
        }
    }
    return self->flag_0x45 ? 5 : 2;
}

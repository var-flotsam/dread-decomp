// CGameManager unit: starts the curve at +0x650 from the current value (+0x6a4) towards `target`
// over `duration` with the modifiers in g_unk_7101d58d10, takes the target at once when the duration
// is 0, then runs fn_7100df6688. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/UnkTimedCurve.h"
#include "types.h"


struct UnkGameManagerFade {
    u8 pad_0x0[0x650];
    UnkTimedCurve curve;  // 0x650
    u8 pad_0x6a0[0x6a4 - 0x650 - sizeof(UnkTimedCurve)];
    f32 value;  // 0x6a4
};

void fn_7100df6688(UnkGameManagerFade* self);

void fn_7100df6c78(UnkGameManagerFade* self, f32 target, f32 duration) {
    UnkTimedCurve* curve = &self->curve;
    asm("" : "+r"(self));  // the original reads the value through the saved copy of self
    fn_7100075f78(self->value, target, duration, curve, &g_unk_7101d58d10);
    if (duration == 0.0f)
        self->value = target;
    fn_7100df6688(self);
}

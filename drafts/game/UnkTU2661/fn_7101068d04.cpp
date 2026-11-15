// Unit 2661 (CCutscene::CTunableCutscene): converts a frame count to seconds, snapping to a whole frame when within 0.01.
// Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkTunable2661 {
    u8 pad_0x0[0xc];
    s32 frames;  // 0xc
};

f32 fn_7101068d04(UnkTunable2661* self) {
    const f32 frameTime = 1.0f / 30.0f;
    f32 seconds = (f32)self->frames * frameTime;
    f32 frames = seconds * 30.0f;
    f32 snapped = roundf(frames * 100.0f) / 100.0f;
    f32 whole = ceilf(snapped);
    f32 result = (whole >= snapped - 0.01f && whole <= snapped + 0.01f) ? whole : snapped;
    return result * frameTime;
}

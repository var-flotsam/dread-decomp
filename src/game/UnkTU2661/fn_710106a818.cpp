// Unit 2661 (CCutscene::CTunableCutscene): the end time in seconds (start frame of the clip plus own offset), snapped to a whole frame when within 0.01.
// Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkClip2661 {
    u8 pad_0x0[8];
    s32 startFrame;  // 0x8
};

struct UnkTunable2661 {
    UnkClip2661* clip;  // 0x0
    u8 pad_0x8[4];
    s32 offset;  // 0xc
};

static inline f32 SnapSeconds(f32 seconds) {
    f32 frames = roundf(seconds * 30.0f * 100.0f) / 100.0f;
    f32 whole = ceilf(frames);
    f32 result = (frames - 0.01f <= whole && frames + 0.01f >= whole) ? whole : frames;
    return result * (1.0f / 30.0f);
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
f32 fn_710106a818(UnkTunable2661* self) {
    return SnapSeconds((f32)(self->clip->startFrame + self->offset) * (1.0f / 30.0f));
}

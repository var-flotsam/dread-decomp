// The base::snd::CSoundSystemATK unit (tools/tu_map.py unit 99): when the fader is enabled (+0x16c)
// and `muted` (+0x16e) changes, starts a fade: from the current value (+0x0, kept at +0x4) towards
// 0 (muted) or 1 (+0x8) over `duration` (+0xc), with the elapsed time (+0x10) reset. Names are
// placeholders.
#include "types.h"

struct UnkAtkFade99 {
    f32 current;   // 0x0
    f32 from;      // 0x4
    f32 to;        // 0x8
    f32 duration;  // 0xc
    f32 elapsed;   // 0x10
    u8 pad_0x14[0x158];
    bool enabled;  // 0x16c
    u8 pad_0x16d;
    bool muted;  // 0x16e
};

void fn_71000f4cec(UnkAtkFade99* self, bool muted, f32 duration) {
    if (self->enabled && self->muted != muted) {
        self->muted = muted;
        f32 to = muted ? 0.0f : 1.0f;
        self->from = self->current;
        self->to = to;
        self->duration = duration;
        self->elapsed = 0.0f;
    }
}

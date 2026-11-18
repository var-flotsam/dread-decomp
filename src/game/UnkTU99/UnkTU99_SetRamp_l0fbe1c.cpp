// Unit 99 (CTunableSoundSystemATK): sets a channel's target value, ramping to it over a duration.
// Names and layouts are placeholders.
#include "base/tunable/CTunable.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkRampTable {
    u8 pad_0x000[0x194];
    f32 start[8];
    f32 slope[8];
    f32 target[8];
    u8 pad_0x1f4[0x347 - 0x1f4];
    bool dirty;
};

void fn_71000fbe1c(UnkRampTable* self, s32 channel, f32 value, f32 duration) {
    fn_7100132cd0();
    f32 current = self->target[channel];
    bool above = current + -0.001f <= value;
    bool below = current + 0.001f >= value;
    bool unchanged = below & above;
    if (!(duration <= 0.0f)) {
        if (unchanged)
            return;
        self->target[channel] = value;
        self->slope[channel] = (value - self->start[channel]) / duration;
        self->dirty = true;
    } else {
        if (unchanged)
            return;
        self->target[channel] = value;
        self->start[channel] = value;
        self->slope[channel] = 0.0f;
        self->dirty = true;
    }
}

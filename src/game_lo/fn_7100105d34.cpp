// Sets a float that can be interpolated: with a positive time it starts a transition from the
// current value to the target (optionally scaled by the previous value), otherwise it jumps
// there. Unless muted, a listener is then told the value now at +0xec (the transition's start,
// or the new value).
#include "types.h"

struct UnkListener_7100105d34;

struct UnkInterp_7100105d34 {
    f32 current;
    f32 start;
    f32 target;
    f32 duration;
    f32 elapsed;
    u8 pad_0x14[0x64];
    UnkListener_7100105d34* listener;
    u8 pad_0x80[0x6c];
    f32 previous;
    u8 pad_0xf0[0x28];
    bool enabled;
    u8 pad_0x119[8];
    bool muted;
};

void fn_71000ffb3c(UnkListener_7100105d34* listener, UnkInterp_7100105d34* block, f32 value);

void fn_7100105d34(UnkInterp_7100105d34* self, f32 value, bool relative, f32 time) {
    if (!self->enabled)
        return;
    if (time > 0.0f) {
        if (relative)
            value = self->previous * value;
        self->target = value;
        self->duration = time;
        self->elapsed = 0.0f;
        self->start = self->current;
        value = self->current;
    } else {
        if (relative)
            value = self->previous * value;
        self->target = value;
        self->start = value;
        self->current = value;
    }
    self->previous = value;
    if (!self->muted && self->listener)
        fn_71000ffb3c(self->listener, self, value);
}

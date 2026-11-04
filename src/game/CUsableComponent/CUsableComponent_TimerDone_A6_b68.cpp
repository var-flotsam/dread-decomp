// CUsableComponent's unit: with the timer flag (+0xf0) set, run the countdown at +0xf4 down by `dt` and report
// whether it has run out; without it, ask fn_7100aa2948. Names are placeholders.
#include "types.h"

struct UnkUsableComponent_A6_b62;
bool fn_7100aa2948(UnkUsableComponent_A6_b62*);

struct UnkUsableTimer_A6_b68 {
    u8 unk_0x0[0xf0];
    bool timed;
    f32 time;
};

bool fn_7100aa301c(UnkUsableTimer_A6_b68* self, f32 dt) {
    if (self->timed) {
        f32 time = self->time;
        if (time > 0.0f) {
            time -= dt;
            self->time = time;
        }
        if (time <= 0.0f)
            return true;
    } else if (fn_7100aa2948(reinterpret_cast<UnkUsableComponent_A6_b62*>(self))) {
        return true;
    }
    return false;
}

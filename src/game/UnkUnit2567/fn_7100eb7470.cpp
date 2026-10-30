// From unit 2567 of tools/tu_map.py (0x7100eb695c-0x7100ebf278, no reflected class): a timed curve
// (base/UnkTimedCurve.h) that can run back and forth. Advances it by `dt` (backwards while the
// direction flag at +0xd4 is clear) and keeps its value at +0xd0; returns whether the time has reached
// the duration. When it bounces (+0xd5), mode 2 turns it round at either end and mode 1 at the end
// only, stepping it back by what it overshot first. Names and layouts are placeholders.
#include "base/UnkTimedCurve.h"

struct UnkBouncingCurve {
    UnkTimedCurve curve;
    u8 pad_0x50[0x78];
    s32 mode;  // 0xc8
    u8 pad_0xcc[4];
    f32 value;     // 0xd0
    bool forward;  // 0xd4
    bool bounce;   // 0xd5
};
static_assert(__builtin_offsetof(UnkBouncingCurve, bounce) == 0xd5, "UnkBouncingCurve layout");

bool fn_7100eb7470(f32 dt, UnkBouncingCurve* self) {
    if (self->bounce) {
        f32 t;
        switch (self->mode) {
        case 1:
            t = self->curve.time;
            if (!(t >= self->curve.duration))
                goto step;
            break;
        case 2:
            t = self->curve.time;
            if (t >= self->curve.duration) {
                self->forward = false;
                t = t - self->curve.duration;
            } else {
                if (!(t <= 0.0f))
                    goto step;
                self->forward = true;
            }
            break;
        default:
            goto step;
        }
        self->value = fn_71000763c0(-t, &self->curve);
    }
step:
    self->value = fn_71000763c0(self->forward ? dt : -dt, &self->curve);
    return self->curve.time >= self->curve.duration;
}

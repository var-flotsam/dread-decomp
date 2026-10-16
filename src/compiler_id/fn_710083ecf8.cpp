// Normalised progress of `current` between `start` and `end`, clamped to (0, 1];
// 0 when inactive or not yet started.
// Not matching: the original's first test is NaN-inclusive (b.lt) and its clamp
// uses fmin; every upstream candidate emits b.mi and fminnm for this source.
#include "types.h"

struct UnkStruct_0x180 {
    u8 unk_0x0[0x170];
    f32 start;
    f32 end;
    bool active;
    f32 current;
};

static inline f32 Min(f32 a, f32 b) {
    return a < b ? a : b;
}

f32 fn_710083ecf8(const UnkStruct_0x180* self) {
    if (!self->active || self->current < self->start)
        return 0.0f;
    f32 t = (self->current - self->start) / (self->end - self->start);
    if (t <= 0.0f)
        return 0.0f;
    return Min(t, 1.0f);
}

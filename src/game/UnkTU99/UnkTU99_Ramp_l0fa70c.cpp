// Unit 99 (base::snd::CSoundSystemATK): moves eight values towards their targets, each at its own speed
// per second, stopping a value (and its speed) once it reaches its target; when any of them moved, marks the
// values dirty (+0x347) and returns true. Names and layouts are placeholders.
#include "types.h"

struct UnkRamps_l0fa70c {
    u8 pad_0x0[0x194];
    f32 values[8];   // 0x194
    f32 speeds[8];   // 0x1b4
    f32 targets[8];  // 0x1d4
    u8 pad_0x1f4[0x153];
    bool dirty;  // 0x347
};
static_assert(__builtin_offsetof(UnkRamps_l0fa70c, dirty) == 0x347, "layout");

static inline bool NearlyEqual(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

bool fn_71000fa70c(UnkRamps_l0fa70c* self, f32 dt) {
    bool moved = false;
    for (int i = 0; i < 8; ++i) {
        f32 speed = self->speeds[i];
        f32 target = self->targets[i];
        if (NearlyEqual(speed, 0.0f))
            continue;
        f32 value = self->values[i];
        value = speed * dt + value;
        if ((speed > 0.0f && value > target) || (speed < 0.0f && target > value)) {
            value = target;
            self->speeds[i] = 0.0f;
        }
        moved = true;
        self->values[i] = value;
    }
    if (moved)
        self->dirty = true;
    return moved;
}

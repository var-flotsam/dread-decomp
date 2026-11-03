// CLockOnMissileMovement unit (hint CCharClassLockOnMissileMovement): one movement step when a
// target is set (+0x30), with the time step clamped by bounds given as (1/30, 1/60), so a step above
// 1/30 s becomes 1/60 s and anything else 1/30 s, passed on with the state at +0x4b8, +0x65c and
// +0x660 (fn_7100ab7774). Names and layouts are placeholders.
#include "types.h"

struct UnkLockOnMissileMovement {
    u8 pad_0x0[0x30];
    void* target;  // 0x30
    u8 pad_0x38[0x480];
    u8 field_0x4b8[0x1a4];
    f32 field_0x65c;
    f32 field_0x660;
};

void fn_7100ab7774(UnkLockOnMissileMovement* self, void* arg, void* state, f32* a, f32* b, f32 dt);

static inline f32 ClampStep(f32 value, f32 low, f32 high) {
    if (value <= low)
        return low;
    return value >= high ? high : value;
}

void fn_7100ab7734(UnkLockOnMissileMovement* self, void* arg, f32 dt) {
    if (self->target)
        fn_7100ab7774(self, arg, self->field_0x4b8, &self->field_0x65c, &self->field_0x660,
                      ClampStep(dt, 1.0f / 30.0f, 1.0f / 60.0f));
}

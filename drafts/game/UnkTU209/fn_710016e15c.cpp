// Unit 209 (a particle-style emitter, no reliable name): advances the emitter's time, updates whether
// it is active and, once past its duration and its next time (+0x14), moves the fraction at +0x18 and the
// next time on by randomly varied steps. Names and layouts are placeholders.
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" int rand();

struct UnkEmitterOwner_l16e15c;

// The emitter as src/game/UnkTU209/UnkTU209_Emitter_ovb0253.cpp lays it out, with its flag a bool.
struct UnkEmitter_l16e15c {
    void** vtable;  // 0x0
    UnkEmitterOwner_l16e15c* owner;  // 0x8
    u8 pad_0x10[0x0];
    f32 time;      // 0x10
    f32 field_0x14;  // 0x14
    f32 field_0x18;  // 0x18
    UnkMatrix44 matrix;  // 0x1c
    UnkVec3f translation;  // 0x5c
    UnkVec3f field_0x68;   // 0x68
    f32 start;       // 0x74
    f32 duration;    // 0x78
    f32 field_0x7c;  // 0x7c
    f32 field_0x80;  // 0x80
    f32 field_0x84;  // 0x84
    f32 field_0x88;  // 0x88
    f32 field_0x8c;  // 0x8c
    f32 field_0x90;  // 0x90
    bool active;     // 0x94
};
static_assert(__builtin_offsetof(UnkEmitter_l16e15c, active) == 0x94, "UnkEmitter_l16e15c layout");

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return lo + (hi - lo) * r;
}

// `value` varied by up to `share` of itself either way (unvaried unless the share is in (0, 1]).
static inline f32 Vary(f32 value, f32 share) {
    f32 offset = 0.0f;
    if (share > 0.0f && share <= 1.0f) {
        f32 range = value * share;
        offset = RandRange(-range, range);
    }
    return value + offset;
}

void fn_710016e15c(UnkEmitter_l16e15c* self, f32 dt) {
    f32 time = self->time + dt;
    self->time = time;
    f32 start = self->start;
    f32 duration = self->duration;
    if (start < 0.0f)
        self->active = true;
    else
        self->active = time <= start + duration;
    if (!self->active || !(time > duration) || !(time > self->field_0x14))
        return;
    // Keep only the fraction of the accumulated steps.
    self->field_0x18 += Vary(self->field_0x7c, self->field_0x80);
    u32 whole = (u32)self->field_0x18;
    if (whole != 0)
        self->field_0x18 -= whole;
    self->field_0x14 += Vary(self->field_0x84, self->field_0x88);
}

// Unit 209 (a particle-style emitter, no reliable name): advances the emitter's time, updates whether
// it is active and, once past its duration and its next time (+0x14), adds a randomly varied step to the
// count at +0x18, emits its whole part through the owner (fn_710016f7f0) and moves the next time on by
// another varied step. Names and layouts are placeholders.
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" int rand();

struct UnkEmitterOwner_b0253;

// Emits `count` particles of the emitter (unit 209).
void fn_710016f7f0(UnkEmitterOwner_b0253* owner, const u32* count, struct UnkEmitter_b0253* emitter);

struct UnkEmitter_b0253 {
    void** vtable;  // 0x0
    UnkEmitterOwner_b0253* owner;  // 0x8
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
    u8 active;       // 0x94
};
static_assert(__builtin_offsetof(UnkEmitter_b0253, active) == 0x94, "UnkEmitter_b0253 layout");

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

void fn_710016e2b4(UnkEmitter_b0253* self, f32 dt) {
    self->time += dt;
    if (self->start < 0.0f)
        self->active = 1;
    else
        self->active = self->time <= self->start + self->duration;
    if (!self->active)
        return;
    if (!(self->time > self->duration))
        return;
    if (!(self->time > self->field_0x14))
        return;
    self->field_0x18 += Vary(self->field_0x7c, self->field_0x80);
    u32 count = (u32)self->field_0x18;
    if (count) {
        fn_710016f7f0(self->owner, &count, self);
        self->field_0x18 -= count;
    }
    self->field_0x14 += Vary(self->field_0x84, self->field_0x88);
}

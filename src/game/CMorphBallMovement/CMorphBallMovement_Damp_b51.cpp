// CMorphBallMovement (unit 1428): damping the velocity at +0x1d8 along a direction. Names and
// layouts are placeholders.
#include "types.h"

struct UnkMorphDampVec {
    f32 x, y, z;
};

struct UnkMorphBallDamp {
    u8 pad_0x0[0x7c];
    f32 field_0x7c;
    u8 pad_0x80[0x1d8 - 0x80];
    UnkMorphDampVec velocity;  // 0x1d8
    u8 pad_0x1e4[0x23c4 - 0x1e4];
    f32 field_0x23c4;
    f32 field_0x23c8;
    f32 field_0x23cc;
    u8 pad_0x23d0[0x6];
    bool field_0x23d6;
};

bool fn_71009438e8(UnkMorphBallDamp* self);

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// Takes the share `dt` x rate of the velocity along `dir` away, without changing its sign.
void fn_7100945238(UnkMorphBallDamp* self, const UnkMorphDampVec* dir, f32 dt) {
    if (!self->field_0x23d6)
        return;
    if (!IsNearZero(self->field_0x7c) && !fn_71009438e8(self))
        return;
    if (!IsNearZero(self->field_0x23c4))
        return;
    f32 along = dir->x * self->velocity.x + dir->y * self->velocity.y + dir->z * self->velocity.z;
    f32 rate = IsNearZero(self->field_0x23c8) ? 6.0f : self->field_0x23cc;
    f32 next = along - along * dt * rate;
    if (along > 0.0f) {
        if (next < 0.0f)
            next = 0.0f;
    } else if (next > 0.0f) {
        next = 0.0f;
    }
    f32 delta = next - along;
    f32 ax = dir->x * delta;
    f32 ay = dir->y * delta;
    f32 az = dir->z * delta;
    self->velocity.x = self->velocity.x + ax;
    self->velocity.y = self->velocity.y + ay;
    self->velocity.z = self->velocity.z + az;
}

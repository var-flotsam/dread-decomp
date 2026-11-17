// CMorphBallMovement's unit (hint only): adds the ball's gravity to its accumulated force (+0x208). The pull
// is down, or against fn_7100949b7c's push direction in modes 2, 4 and 5, none in mode 3; on a contact it
// is kept along the surface (normal `normal`) while +0x3a8 is set or, with +0x3ac zero, the ball moves.
// Names and layouts are placeholders.
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMorphBallMovement_l949b7c;
UnkVec3f fn_7100949b7c(UnkMorphBallMovement_l949b7c* self);

struct UnkGravityLink_l945008 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkGravityBall_l945008 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkVec3f velocity;  // 0x40
    u8 pad_0x4c[0xf9 - 0x4c];
    u8 field_0xf9;
    u8 pad_0xfa[0x128 - 0xfa];
    UnkGravityLink_l945008* contact;  // 0x128
    u8 pad_0x130[0x1dc - 0x130];
    f32 field_0x1dc;
    u8 pad_0x1e0[0x208 - 0x1e0];
    UnkVec3f force;  // 0x208
    u8 pad_0x214[0x3a8 - 0x214];
    u8 field_0x3a8;
    u8 pad_0x3a9[0x3];
    f32 field_0x3ac;
    u8 pad_0x3b0[0x2288 - 0x3b0];
    s32 mode;  // 0x2288
    u8 pad_0x228c[0x22b0 - 0x228c];
    f32 field_0x22b0;
    u8 pad_0x22b4[0x23b1 - 0x22b4];
    u8 field_0x23b1;
    u8 field_0x23b2;

    bool vf_0x180() { return reinterpret_cast<bool (*)(UnkGravityBall_l945008*)>(vtable[0x180 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkGravityBall_l945008, field_0x23b2) == 0x23b2, "layout");

inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

inline bool IsNearZero(const UnkVec3f& v) {
    return IsNearZero(v.x) && IsNearZero(v.y) && IsNearZero(v.z);
}

void fn_7100945008(UnkGravityBall_l945008* self, const UnkVec3f* normal) {
    if (!self->vf_0x180())
        return;

    f32 strength = 3700.0f;
    if (!self->field_0x23b2 && self->field_0x23b1 && self->field_0x1dc > 0.0f)
        strength = self->field_0x22b0 * 3700.0f;

    UnkVec3f dir(0.0f, -1.0f, 0.0f);
    switch (self->mode) {
    case 2:
    case 4:
    case 5:
        dir = -fn_7100949b7c(reinterpret_cast<UnkMorphBallMovement_l949b7c*>(self));
        break;
    }
    if (self->mode == 3)
        dir = UnkVec3f(0.0f, 0.0f, 0.0f);

    UnkVec3f force(strength * dir.x, strength * dir.y, strength * dir.z);
    if (!self->field_0xf9 && self->contact && self->contact->target &&
        (self->field_0x3a8 || (self->field_0x3ac == 0.0f && !IsNearZero(self->velocity))))
        force = force - *normal * Dot(force, *normal);
    self->force = force + self->force;
}

// Unit 1428 (CMorphBallMovement's neighbours): turns the ball's rotation about z by the distance it
// moved from `from` to `to` over its circumference (six times as fast when flagged), forward or back by
// the way it moves in its current mode, then adds `step` and normalises the angles.
// Names and layouts are placeholders.
#include <math.h>
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBallOwner_l942e14 {
    u8 pad_0x0[0x37c];
    f32 radius;  // +0x37c
};

struct UnkBallContact_l942e14 {
    u8 pad_0x0[0x10];
    void* surface;  // +0x10
};

struct UnkBallMovement_l942e14 {
    u8 pad_0x0[0x30];
    UnkBallOwner_l942e14* owner;  // +0x30
    u8 pad_0x38[0xfa - 0x38];
    bool field_0xfa;
    u8 pad_0xfb[0x128 - 0xfb];
    UnkBallContact_l942e14* contact;  // +0x128
    u8 pad_0x130[0x148 - 0x130];
    f32 normal_x;  // +0x148
    f32 normal_y;  // +0x14c
    u8 pad_0x150[0x6f4 - 0x150];
    bool field_0x6f4;
    u8 pad_0x6f5[0x6fc - 0x6f5];
    f32 field_0x6fc;
    u8 pad_0x700[0x2288 - 0x700];
    s32 mode;  // +0x2288
    u8 pad_0x228c[0x2310 - 0x228c];
    f32 field_0x2310;
    u8 pad_0x2314[0x2480 - 0x2314];
    s32 field_0x2480;
    u8 pad_0x2484[0x2544 - 0x2484];
    f32 field_0x2544;
    u8 pad_0x2548[0x254c - 0x2548];
    bool fast;  // +0x254c
    u8 pad_0x254d[0x2554 - 0x254d];
    f32 field_0x2554;

    bool IsRolling() const { return mode == 4 || mode == 5 || mode == 2; }
    bool HasSurface() const { return contact && contact->surface; }
    // The turns a distance rolled makes: six times as many when fast.
    f32 Turns(f32 dist, f32 circumference) const {
        f32 turns = dist / circumference;
        return fast ? turns * 6.0f : turns;
    }
};

static constexpr f64 kTwoPi_l942e14 = 6.283185307179586;

// Turns the rotation about z back or forward by a number of turns.
inline void Roll_l942e14(UnkVec3f* rot, f32 turns, bool back, bool forward) {
    if (back)
        rot->z -= turns * kTwoPi_l942e14;
    else if (forward)
        rot->z += turns * kTwoPi_l942e14;
}

void fn_7100942e14(UnkBallMovement_l942e14* self, const UnkVec3f* from, const UnkVec3f* to, UnkVec3f* rot, f32 step) {
    f32 dx = to->x - from->x;
    f32 dy = to->y - from->y;
    f32 dz = to->z - from->z;
    f32 circumference = self->owner->radius * 6.2831855f;

    f32 turns = 0.0f;
    bool back = false;
    bool forward = false;
    if (self->IsRolling() && self->field_0x2544 > 0.0f && self->HasSurface()) {
        f32 len = sqrtf(dx * dx + dy * dy + dz * dz);
        if (self->field_0x2480 == 14) {
            turns = self->Turns(len, circumference);
            f32 side = dx * self->normal_y - dy * self->normal_x;
            back = side < 0.0f;
            forward = side > 0.0f;
        }
    } else if (self->IsRolling() && self->HasSurface() && !self->field_0xfa) {
        f32 len = sqrtf(dx * dx + dy * dy + dz * dz);
        turns = self->Turns(len, circumference);
        f32 side = dx * self->normal_y - dy * self->normal_x;
        back = side < 0.0f;
        forward = side > 0.0f;
    } else if (self->mode == 3) {
        f32 len = sqrtf(dx * dx + dy * dy + dz * dz);
        turns = self->Turns(fabsf(len), circumference);
        back = self->field_0x2554 <= 0.0f;
        forward = self->field_0x2554 > 0.0f;
    } else if (self->field_0x6f4) {
        f32 base = self->field_0x2310 != 0.0f ? fabsf(dx) : 0.0f;
        f32 dist = base + self->field_0x6fc * 2.0f * sinf(step * 0.5f);
        turns = self->Turns(fabsf(dist), circumference);
        back = self->field_0x2310 > 0.0f;
        forward = self->field_0x2310 < 0.0f;
    } else {
        turns = self->Turns(fabsf(dx), circumference);
        back = to->x > from->x;
        forward = to->x < from->x;
    }
    Roll_l942e14(rot, turns, back, forward);
    rot->z += step;
    *rot = fn_710007a220(rot);
}
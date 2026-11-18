// CMorphBallMovement's unit (tools/tu_map.py unit 1428): the reset on leaving the ball. Drops the
// counted object at +0x2628 (clearing its flag first), clears the flag at +0x2538 in state 1 or else runs
// fn_710093efdc, hands the value vf_0x2a8 gives to vf_0x2b0, zeroes the ball's motion state, runs
// fn_710093fab8, releases the preset ids at +0x2370 and ends with the base (fn_710097973c).
// Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "types.h"

struct UnkMorphBall_eb64;
void fn_710093efdc(UnkMorphBall_eb64* self, u32 a, u32 b, u32 c);
struct UnkCharacterMovementOwner_ovb0021;
void fn_710097973c(UnkCharacterMovementOwner_ovb0021* self);

// A counted object the ball holds while it is in use.
struct UnkBallHeld_l93f98c {
    virtual void vf_0x0();
    virtual void Delete();
    u8 pad_0x8[0x1c];
    bool in_use;  // 0x24
    u8 pad_0x25[0xb];
    s32 ref_count;  // 0x30
};

struct UnkBallRoll_l93f98c {
    f32 field_0x0[4];
    u8 pad_0x10[0x84];
    f32 field_0x94[2];
    bool active;  // 0x9c
    u8 pad_0x9d[2];
    bool field_0x9f;
};
static_assert(sizeof(UnkBallRoll_l93f98c) == 0xa0, "layout");

struct UnkMorphBall_l93f98c {
    void** vtable;
    u8 pad_0x8[0x6e8 - 0x8];
    f32 velocity[3];  // 0x6e8
    u8 pad_0x6f4[0x2288 - 0x6f4];
    s32 state;  // 0x2288
    u8 pad_0x228c[0x2370 - 0x228c];
    UnkStrIdPathResource preset;  // 0x2370
    u8 pad_0x2388[0x249c - 0x2388];
    UnkBallRoll_l93f98c roll;  // 0x249c
    f32 field_0x253c;
    u8 pad_0x2540[0x2628 - 0x2540];
    UnkBallHeld_l93f98c* held;  // 0x2628

    void* vf_0x2a8() { return reinterpret_cast<void* (*)(UnkMorphBall_l93f98c*)>(vtable[0x2a8 / 8])(this); }
    void vf_0x2b0(void* value) {
        reinterpret_cast<void (*)(UnkMorphBall_l93f98c*, void*)>(vtable[0x2b0 / 8])(this, value);
    }
};

void fn_710093fab8(UnkMorphBall_l93f98c* self, bool a, f32 b);

void fn_710093f98c(UnkMorphBall_l93f98c* self) {
    if (self->held) {
        self->held->in_use = false;
        UnkBallHeld_l93f98c* held = self->held;
        if (held) {
            if (--held->ref_count == 0)
                held->Delete();
            self->held = nullptr;
        }
    }
    if (self->state == 1) {
        if (self->roll.active)
            self->roll.active = false;
    } else {
        fn_710093efdc(reinterpret_cast<UnkMorphBall_eb64*>(self), 1, 0, 1);
    }
    self->vf_0x2b0(self->vf_0x2a8());
    self->velocity[0] = 0.0f;
    self->velocity[1] = 0.0f;
    self->velocity[2] = 0.0f;
    self->roll.field_0x9f = false;
    self->field_0x253c = 0.0f;
    self->roll.field_0x0[0] = 0.0f;
    self->roll.field_0x0[1] = 0.0f;
    self->roll.field_0x0[2] = 0.0f;
    self->roll.field_0x0[3] = 0.0f;
    self->roll.active = false;
    self->roll.field_0x94[0] = 0.0f;
    self->roll.field_0x94[1] = 0.0f;
    fn_710093fab8(self, false, 0.0f);
    fn_7100027044(&self->preset);
    fn_710097973c(reinterpret_cast<UnkCharacterMovementOwner_ovb0021*>(self));
}

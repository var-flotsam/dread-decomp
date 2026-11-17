// build-flags: no-strict-aliasing
// CCameraCtrl's unit (tools/tu_map.py, unit 2693): advances a camera controller by `dt`: its clock, the
// shared timer of what it follows (in the modes that run on a period), its blend weight and progress,
// then runs its update (vf_0x48) and its finish step (vf_0xd0). Names and layouts are placeholders.
#include "types.h"

// What the controller follows: a rate and a timer.
struct UnkCameraFollowed_l0edeb4 {
    u8 pad_0x0[0x25c];
    f32 rate;   // 0x25c
    f32 timer;  // 0x260
};

struct UnkCameraFollow_l0edeb4 {
    u8 pad_0x0[0x10];
    UnkCameraFollowed_l0edeb4* followed;  // 0x10
};

struct UnkCameraCtrl_l0edeb4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(f32 dt, bool flag);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();

    u8 pad_0x8[0x2c - 0x8];
    f32 clock;  // 0x2c
    u8 pad_0x30[0x9c - 0x30];
    s32 mode;      // 0x9c
    f32 progress;  // 0xa0
    f32 period;    // 0xa4
    f32 weight;    // 0xa8
    u8 pad_0xac[0x4];
    UnkCameraFollow_l0edeb4* follow;  // 0xb0
};
static_assert(__builtin_offsetof(UnkCameraCtrl_l0edeb4, follow) == 0xb0, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010edeb4(UnkCameraCtrl_l0edeb4* self, f32 dt) {
    self->clock += dt;
    bool update = true;
    if (self->mode == 0) {
        self->follow->followed->timer += dt;
        if (self->follow->followed->timer < self->period) {
            update = false;
        } else {
            self->follow->followed->timer -= self->period;
            self->progress = self->follow->followed->timer;
        }
    } else if (self->mode == 1) {
        self->follow->followed->timer += dt;
        if (self->follow->followed->timer < self->period)
            update = false;
        else
            self->follow->followed->timer = 0.0f;
    }
    if (update)
        self->vf_0x48(dt, false);

    self->weight = self->follow->followed->rate * dt;
    if (self->mode == 0) {
        self->progress = (self->progress + dt) * (1.0f - self->period);
        f32 progress = self->progress * self->follow->followed->rate;
        self->progress = !(progress >= 1.0f) ? progress : 1.0f;
        self->weight = self->progress;
    } else if (self->mode == 2) {
        self->weight = 1.0f;
    }
    self->vf_0xd0();
}

// CAimComponent (tools/tu_map.py): the per-frame update with the frame time `dt`. Advances the clock at
// +0x67c (with fn_710034f934 while +0x6a2 is set), runs fn_710034a7b4, winds the angle at +0x2d0 by a
// turn per second up to the tunable's (+0x2b0) limit at +0x7c, steps the aim (fn_71003460ec on +0x580,
// fn_7100349f28) and, in state 7 (+0x69c) when the animation (+0x220 of the owner at +0x30) plays none
// of three actions, moves to state 6; in state 6 runs fn_7100350da0. Ends with fn_7100347540. Names
// and layouts are placeholders.
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkSelf;
void fn_7100350da0(UnkSelf*);

struct UnkAimTunable_b49 {
    u8 pad_0x0[0x7c];
    f32 max_angle;
};

struct UnkAimOwner_b49 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkAimUpdate_b49 {
    u8 pad_0x0[0x30];
    UnkAimOwner_b49* owner;
    u8 pad_0x38[0x278];
    UnkAimTunable_b49* tunable;
    u8 pad_0x2b8[0x18];
    f32 angle;
    u8 pad_0x2d4[0x2ac];
    u8 field_0x580[0xfc];
    f32 clock;
    u8 pad_0x680[0x1c];
    s32 state;
    u8 pad_0x6a0[2];
    bool field_0x6a2;
};

void fn_710034f934(UnkAimUpdate_b49* self, u32 flag, f32 clock);
void fn_710034a7b4(UnkAimUpdate_b49* self, f32 value);
f32 fn_71003460ec(UnkAimUpdate_b49* self, void* aim, u32 mode, f32 dt);
void fn_7100349f28(UnkAimUpdate_b49* self, f32 dt, f32 value);
void fn_7100347540(UnkAimUpdate_b49* self);

void fn_7100350c5c(UnkAimUpdate_b49* self, f32 dt) {
    self->clock += dt;
    if (self->field_0x6a2)
        fn_710034f934(self, 1, self->clock);
    fn_710034a7b4(self, -1.0f);
    f32 angle = dt * 6.2831855f + self->angle;
    self->angle = angle;
    f32 max = self->tunable->max_angle;
    if (angle > max)
        self->angle = max;
    f32 value = fn_71003460ec(self, self->field_0x580, 6, dt);
    fn_7100349f28(self, dt, value);
    s32 state = self->state;
    if (state == 7) {
        if (!fn_710117ae38(self->owner->animation, 0xce302f5ecc6dd2daull, false) &&
            !fn_710117ae38(self->owner->animation, 0xf335a18d7b2cf40full, false) &&
            !fn_710117ae38(self->owner->animation, 0xaafccaf5fb3e9a4cull, false)) {
            self->state = 6;
            fn_7100350da0(reinterpret_cast<UnkSelf*>(self));
            fn_7100347540(self);
            return;
        }
        state = self->state;
    }
    if (state == 6)
        fn_7100350da0(reinterpret_cast<UnkSelf*>(self));
    fn_7100347540(self);
}

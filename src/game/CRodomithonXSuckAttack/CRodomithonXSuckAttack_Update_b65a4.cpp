// CRodomithonXSuckAttack's unit (tools/tu_map.py): the attack's update. In state 1 the timer at +0x34
// counts down by the step and moves to state 0 (fn_7100664724) when it runs out; in state 0 the timer
// at +0x38 does the same and moves to state 1. Unless the byte at +0x121e of the owner's AI component
// (+0x240 of the owner at +0x8) is set, it moves to state 2; then the base update (fn_71003609a8) runs
// with the step. Names and layouts are placeholders.
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkSuckAI_b65a4 {
    u8 pad_0x0[0x121e];
    bool field_0x121e;
};

struct UnkSuckOwner_b65a4 {
    u8 pad_0x0[0x240];
    UnkSuckAI_b65a4* ai;  // 0x240
};

struct UnkSuckAttack_b65a4 {
    u8 pad_0x0[0x8];
    UnkSuckOwner_b65a4* owner;  // 0x8
    u8 pad_0x10[0x20];
    s32 state;  // 0x30
    f32 timer1;  // 0x34
    f32 timer0;  // 0x38
};

void fn_7100664724(UnkSuckAttack_b65a4* self, s32 state);

void fn_7100664bcc(UnkSuckAttack_b65a4* self, f32 dt) {
    switch (self->state) {
    case 0:
        self->timer0 -= dt;
        if (self->timer0 < 0.0f)
            fn_7100664724(self, 1);
        break;
    case 1:
        self->timer1 -= dt;
        if (self->timer1 < 0.0f)
            fn_7100664724(self, 0);
        break;
    }
    if (!self->owner->ai->field_0x121e)
        fn_7100664724(self, 2);
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
}

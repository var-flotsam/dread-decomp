// CBigkranXMagmaRainAttack's unit (the name from reflection): the attack's update. After the base update
// (fn_71003609a8) the pending state (+0x34) becomes current (+0x30); state 2 counts its timer (+0x58) down by `dt`
// and moves to state 3 (fn_71003ea6d8) when it runs out, else (and in state 1) runs fn_71003ea960.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSpitAttack;
void fn_71003609a8(UnkSpitAttack*);

struct UnkMagmaRainAttack_A2_b69 {
    u8 pad_0x0[0x30];
    s32 state;    // +0x30
    s32 pending;  // +0x34
    u8 pad_0x38[0x20];
    f32 timer;  // +0x58
};

void fn_71003ea6d8(UnkMagmaRainAttack_A2_b69* self, s32 state);
void fn_71003ea960(UnkMagmaRainAttack_A2_b69* self, f32 dt);

void fn_71003ea8cc(UnkMagmaRainAttack_A2_b69* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self));
    s32 state = self->pending;
    self->state = state;
    if (state == 1) {
        fn_71003ea960(self, dt);
    } else if (state == 2) {
        self->timer -= dt;
        if (self->timer < 0.001f)
            fn_71003ea6d8(self, 3);
        else
            fn_71003ea960(self, dt);
    }
}

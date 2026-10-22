// CBigkranXSpitAttack's update: runs the base's part (fn_71003609a8), makes the requested state
// (+0x34) the current one (+0x30), then runs that state: the state-2 update, or, in state 1, the
// state-1 update and, once it reports it is done, the switch to state 3.
#include "types.h"

struct UnkSpitAttackState {
    u8 pad_0x0[0x30];
    s32 field_0x30;
    s32 field_0x34;
};

void fn_71003609a8(UnkSpitAttackState* self);
bool fn_71003f28c8(UnkSpitAttackState* self, f32 delta);
void fn_71003f24f4(UnkSpitAttackState* self, s32 state);
void fn_71003f2710(UnkSpitAttackState* self, f32 delta);

void fn_71003f268c(UnkSpitAttackState* self, f32 delta) {
    fn_71003609a8(self);
    s32 state = self->field_0x34;
    self->field_0x30 = state;
    switch (state) {
    case 1:
        if (fn_71003f28c8(self, delta))
            fn_71003f24f4(self, 3);
        break;
    case 2:
        fn_71003f2710(self, delta);
        break;
    }
}

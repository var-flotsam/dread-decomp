// A CTakumakuDashAttack method: runs the base's part (fn_71003609b8), clears +0x38, then
// unregisters and releases the listener's owner (listener at +0x40, owner at +0x58). The shape of
// fn_71003f38f8.
#include "../../game_5821e4/CAttackDtor.h"
#include "game/UnkAttack.h"

struct UnkTakumakuDashAttackReset : UnkAttackBase {
    u8 pad_0x20[0x18];
    u64 field_0x38;
    UnkAttackListenerRaw field_0x40;
};

void fn_71006d06bc(UnkTakumakuDashAttackReset* self) {
    fn_71003609b8(self);
    self->field_0x38 = 0;
    UnkAttackListenerRaw& listener = self->field_0x40;
    if (UnkAttackOwner* o = listener.owner) {
        if (o->field_0x10)
            fn_710117b3f0(o->field_0x10, &listener);
        if (UnkAttackOwner* p = listener.owner) {
            if (--p->ref_count == 0)
                p->Delete();
            listener.owner = nullptr;
        }
    }
}

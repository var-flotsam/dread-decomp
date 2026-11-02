// A CSluggerSpitAttack method: runs the base's part (fn_71003609b8), unregisters and releases the
// listener's owner (listener at +0x38, owner at +0x50), hands the actor's object at +0x2d0 (when
// there is one) to fn_71009f92d0, then runs fn_71006c0930. The shape of fn_71003f38f8.
#include "../../game_5821e4/CAttackDtor.h"
#include "game/UnkAttack.h"

void fn_71009f92d0(void* object);

struct UnkSluggerAttackOwnerObject {
    u8 pad_0x0[0x2d0];
    void* field_0x2d0;
};

struct UnkSluggerSpitAttackReset : UnkAttackBase {
    u8 pad_0x20[0x18];
    UnkAttackListenerRaw field_0x38;
};

void fn_71006c0930(UnkSluggerSpitAttackReset* self);

void fn_71006c39cc(UnkSluggerSpitAttackReset* self) {
    fn_71003609b8(self);
    UnkAttackListenerRaw& listener = self->field_0x38;
    if (UnkAttackOwner* o = listener.owner) {
        if (o->field_0x10)
            fn_710117b3f0(o->field_0x10, &listener);
        if (UnkAttackOwner* p = listener.owner) {
            if (--p->ref_count == 0)
                p->Delete();
            listener.owner = nullptr;
        }
    }
    if (auto* owner = reinterpret_cast<UnkSluggerAttackOwnerObject*>(self->field_0x8)) {
        if (owner->field_0x2d0)
            fn_71009f92d0(owner->field_0x2d0);
    }
    fn_71006c0930(self);
}

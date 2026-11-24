// Constructor tail of a CBigkranXSpitAttack's component: runs the base (fn_71003606c8), and, given
// an owner's object, stores it cast to the attack class (null when it is not one) and, when that
// succeeds, clears +0xd0. The state at +0xe8 to +0xf4 (a float at +0xf4) is reset in every case.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkSelf.h"

using base::reflection::CClass;

struct UnkActor_71003b4718 {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkActor_71003b4718*)>(vtable[0])(this); }
};


void fn_71003606c8(UnkSelf* self, UnkActor_71003b4718* actor);

void fn_71003f14c4(UnkSelf* self, UnkActor_71003b4718* actor) {
    fn_71003606c8(self, actor);
    if (actor) {
        UnkActor_71003b4718* attack = nullptr;
        if (fn_710009d50c(actor->vf_0x0(), fn_71003ef9b4()))
            attack = actor;
        self->field_0x28 = attack;
        if (attack)
            self->field_0xd0 = 0;
    }
    // reset whatever the actor is, also without one
    self->field_0xe8 = 0;
    self->field_0xf0 = 0x4426800000000000;
}

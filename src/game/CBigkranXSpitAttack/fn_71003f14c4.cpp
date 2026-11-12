// Constructor tail of a CBigkranXSpitAttack's component: runs the base (fn_71003606c8), and, given
// an owner's object, stores it cast to the attack class (null when it is not one) and, when that
// succeeds, clears +0xd0. The state at +0xe8 to +0xf4 (a float at +0xf4) is reset in every case.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkActor {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkActor*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[0x28];
    UnkActor* field_0x28;
    u8 pad_0x30[0xa0];
    f32 field_0xd0;
    u8 pad_0xd4[0x14];
    u64 field_0xe8;
    u64 field_0xf0;
};

void fn_71003606c8(UnkSelf* self, UnkActor* actor);

void fn_71003f14c4(UnkSelf* self, UnkActor* actor) {
    fn_71003606c8(self, actor);
    if (actor) {
        UnkActor* attack = nullptr;
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

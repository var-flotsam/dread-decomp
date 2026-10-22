// Constructor tail of a CBigkranXSpitAttack's component: runs the base (fn_71003606c8), and, given
// an owner's object, stores it cast to the attack class (null when it is not one). When that
// succeeds the component's state is reset (+0xd0, +0xe8/+0xec/+0xf0, a float at +0xf4).
#include "types.h"

struct UnkActor {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkActor*)>(vtable[0])(this); }
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
void* fn_71003ef9b4();  // CCharClassBigkranXSpitAttack::refl_GetClass
bool fn_710009d50c(const void* from, void* to);

// NON_MATCHING(minor): same behaviour; the original stores +0xd0 before building the 64-bit constant
// for +0xf0, ours builds it first.
void fn_71003f14c4(UnkSelf* self, UnkActor* actor) {
    fn_71003606c8(self, actor);
    if (actor) {
        UnkActor* attack = nullptr;
        if (fn_710009d50c(actor->vf_0x0(), fn_71003ef9b4()))
            attack = actor;
        self->field_0x28 = attack;
        if (attack) {
            self->field_0xd0 = 0;
            self->field_0xe8 = 0;
            self->field_0xf0 = 0x4426800000000000;
        }
    }
}

// A CBigkranXSpitAttack method: runs the base's part (fn_71003609b8), clears the state it keeps
// (+0x30, +0xec/+0xf0, a float at +0xf4), unregisters and releases the listener's owner
// (listener at +0x38, owner at +0x50), then, when the attack's own owner holds an object at
// +0x2d0, hands that to fn_71009f92d0.
#include "../../game_5821e4/CAttackDtor.h"
#include "game/UnkAttack.h"

void fn_71009f92d0(void* object);

struct UnkAttackOwnerObject {
    u8 pad_0x0[0x2d0];
    void* field_0x2d0;
};

struct UnkSpitAttackReset : UnkAttackBase {
    u8 pad_0x20[0x10];
    u64 field_0x30;
    UnkAttackListenerRaw field_0x38;
    u8 pad_0x58[0x94];
    u32 field_0xec[2];
    f32 field_0xf4;
};

void fn_71003f38f8(UnkSpitAttackReset* self) {
    fn_71003609b8(self);
    self->field_0x30 = 0;
    self->field_0xec[0] = 0;
    self->field_0xec[1] = 0;
    self->field_0xf4 = 666.0f;
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
    if (auto* owner = reinterpret_cast<UnkAttackOwnerObject*>(self->field_0x8)) {
        if (owner->field_0x2d0)
            fn_71009f92d0(owner->field_0x2d0);
    }
}

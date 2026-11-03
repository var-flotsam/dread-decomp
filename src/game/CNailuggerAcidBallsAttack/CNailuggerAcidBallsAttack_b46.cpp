// The Nailugger acid-balls attack's state setter (tools/tu_map.py: the CNailuggerAcidBallsAttack unit):
// on a change of state (+0x50), takes the owner's Nailong AI component (+0x8, then +0x240) when it is
// one, then per state: 1 clears +0x68 and starts one action, 3 starts another, 2 asks the component for
// two values (+0x70, +0x78). Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAcidBallsOwner_b46a2 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;
};

struct UnkAcidBallsAttack_b46a2 {
    u8 pad_0x0[0x8];
    UnkAcidBallsOwner_b46a2* owner;
    u8 pad_0x10[0x40];
    u32 state;
    u8 pad_0x54[0x14];
    u32 field_0x68;
    u8 pad_0x6c[0x4];
    u32 field_0x70;
    u8 pad_0x74[0x4];
    u32 field_0x78;
};

u32 fn_7100621a5c(UnkReflObject* ai);
u32 fn_7100621ac0(UnkReflObject* ai, u32 value);
void fn_7100625478(UnkAcidBallsAttack_b46a2* self, u64 key);

void fn_7100624fe0(UnkAcidBallsAttack_b46a2* self, u32 state) {
    if (self->state == state)
        return;
    self->state = state;
    UnkReflObject* ai = self->owner->ai;
    if (ai)
        ai = fn_710009d50c(ai->vf_0x0(), fn_710061f464()) ? ai : nullptr;
    switch (self->state) {
    case 1:
        self->field_0x68 = 0;
        fn_7100625478(self, 0xce6cb58bea34b4f7);
        break;
    case 3:
        fn_7100625478(self, 0xf1a8560f74132841);
        break;
    case 2: {
        u32 v = fn_7100621a5c(ai);
        self->field_0x70 = v;
        self->field_0x78 = fn_7100621ac0(ai, v);
        break;
    }
    }
}

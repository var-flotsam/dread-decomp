// Per-frame step of the flee action: runs the base step, clears the two flags at +0x59, and unless the
// owner's movement object (+0x18, +0x1f0, when it is a CEnemyMovement) is in state 4, hands a default
// parameter block to fn_71006f4b0c with 1.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"

using base::reflection::UnkReflObject;

struct UnkOwner {
    u8 pad_0x0[0x1f0];
    UnkReflObject* field_0x1f0;
};

struct UnkFleeAction {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
    u8 pad_0x20[0x18];
    void* field_0x38;
    u8 pad_0x40[0x19];
    u8 field_0x59;
    u8 field_0x5a;
};

void fn_71005c8780(UnkFleeAction* self) {
    fn_7100394bd4(self);
    UnkOwner* owner = self->field_0x18;
    *reinterpret_cast<u16*>(&self->field_0x59) = 0;
    UnkReflObject* movement = owner->field_0x1f0;
    UnkReflObject* cast = nullptr;
    if (movement)
        cast = fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    if (fn_71010cc3a4(cast) != 4) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x34 = 5;
        params.field_0x30 = 0;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(self->field_0x38, &params, 1);
    }
}

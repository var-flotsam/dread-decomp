// Unit 2689 (CPlayerMovement hint): with the player busy, finds the collision component of the actor behind
// the argument, takes its record's linked object and asks the frost-bite component whether it is frozen but
// not thawing. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkRecord_l0dcab0 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};
struct UnkLinked_l0dcab0;
struct UnkCollision_l0dcab0 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLinked_l0dcab0* field_0x18;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkCollision_l0dcab0*)>(vtable[0])(this);
    }
};
struct UnkHolder_l0dcab0 {
    u8 pad_0x0[0x10];
    UnkCollision_l0dcab0* field_0x10;
};
struct UnkHolderPtr_l0dcab0 {
    u8 pad_0x0[0x10];
    UnkHolder_l0dcab0* field_0x10;
};
struct UnkArg_l0dcab0 {
    UnkHolderPtr_l0dcab0* field_0x0;
};

UnkRecord_l0dcab0* fn_71010dcb7c(UnkLinked_l0dcab0* linked);
UnkFrostbiteComponent* fn_710032bc90(UnkLinked_l0dcab0* linked);

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
u32 fn_71010dcab0(void*, UnkArg_l0dcab0* arg) {
    if (!fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)) && !fn_7100df5a4c(g_unk_7101d47310))
        return false;
    UnkCollision_l0dcab0* collision = arg->field_0x0->field_0x10->field_0x10;
    if (!collision || !fn_710009d50c(collision->vf_0x0(), fn_710085fc44()))
        return false;
    UnkLinked_l0dcab0* linked = collision->field_0x18;
    if (!linked)
        return false;
    UnkRecord_l0dcab0* record = fn_71010dcb7c(linked);
    if (!record || record->field_0x70 == 2)
        return false;
    UnkFrostbiteComponent* frost = fn_710032bc90(linked);
    if (!frost || !fn_71007f9f20(frost))
        return false;
    return !fn_71007f9fd0(frost);
}

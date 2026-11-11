// CInfesterReloadAttack's unit (tools/tu_map.py unit 886): the base attack's check (fn_7100360b84),
// and the owner's movement (+0x1f0, a CEnemyMovement) in mode 1 (fn_71010cc3a4); then the flag at
// +0x146dc of the owner's CInfesterAIComponent (+0x240). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"
#include "game/CAttack.h"

// As src/game/behaviortree/CInfesterFleeFromTargetAction/CInfesterFleeFromTargetAction.cpp declares it.

struct UnkInfesterAttack_A3_b67 {
    void** vtable;
    UnkActor* owner;
};

// `arg` and `mode` (x1, w2) reach the base check untouched in the original (tools/forwarded_args.py).
u8 fn_71005c55f4(UnkInfesterAttack_A3_b67* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode);
    auto* ai = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    u8* infester = ai && fn_710009d50c(ai->vf_0x0(), fn_71005ca678()) ? reinterpret_cast<u8*>(ai) : nullptr;
    auto* mv = reinterpret_cast<base::reflection::UnkReflObject*>(reinterpret_cast<void**>(self->owner)[0x1f0 / 8]);
    void* movement = mv && fn_710009d50c(mv->vf_0x0(), fn_71010c74e0()) ? mv : nullptr;
    if ((fn_71010cc3a4(movement) == 1) & ok)
        return infester[0x146dc];
    return false;
}

// querysystem::CCurrentEvaluator unit (tools/tu_map.py, the name from reflection): scores a candidate 1 when it is
// the current one (+0x27760) of the actor's AI component (+0x240), which must be a CChozoRobotSoldierAIComponent
// (fn_710046ea2c); otherwise 0. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkCurrentEvaluatorAI_b67a2 {
    void** vtable;
    u8 pad_0x8[0x27760 - 0x8];
    void* current;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkCurrentEvaluatorAI_b67a2*)>(vtable[0])(this);
    }
};

struct UnkCurrentEvaluatorActor_b67a2 {
    u8 pad_0x0[0x240];
    UnkCurrentEvaluatorAI_b67a2* ai;
};

f32 fn_71002bab54(void* self, UnkCurrentEvaluatorActor_b67a2* actor, void* candidate) {
    UnkCurrentEvaluatorAI_b67a2* ai = actor->ai;
    if (ai != nullptr && fn_710009d50c(ai->vf_0x0(), fn_710046ea2c()) && ai->current == candidate)
        return 1.0f;
    return 0.0f;
}

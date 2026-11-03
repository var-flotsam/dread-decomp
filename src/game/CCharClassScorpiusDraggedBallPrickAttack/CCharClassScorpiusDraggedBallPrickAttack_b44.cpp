// CCharClassScorpiusDraggedBallPrickAttack (tools/tu_map.py; the name from reflection): a distance
// for the attack. The actor's CScorpiusAIComponent (+0x240, cast) reports two values through its
// vf_0x5e0 (the second, then the first); the target's x (fn_7100d7c990, +0x44) minus the first is
// negated when first < second. Below the Scorpius tunable's value at +0x28 that limit is
// returned, else the distance plus 15. Names and layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_7172701a80;  // the "Scorpius" tunable

struct UnkScorpiusTunable_b44a2 {
    u8 pad_0x0[0x28];
    f32 field_0x28;
};

f32 fn_710067fa34(void* self, UnkActor* actor) {
    using base::reflection::UnkReflObject;
    auto* ai = static_cast<UnkReflObject*>(actor->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_7100690c74()))
        ai = nullptr;
    f32 second = 0.0f;
    f32 first = 0.0f;
    using Get = void (*)(UnkReflObject*, f32*, f32*);
    (*reinterpret_cast<Get**>(ai))[0x5e0 / 8](ai, &second, &first);
    f32 d = fn_7100d7c990(actor)->field_0x44 - first;
    f32 v = first < second ? -d : d;
    f32 limit = reinterpret_cast<UnkScorpiusTunable_b44a2*>(&g_unk_7172701a80)->field_0x28;
    return v < limit ? limit : v + 15.0f;
}

// CCharClassInfesterBallLifeComponent's unit (tools/tu_map.py, unit 896). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkInfesterBallAI_ovb0130 {
    u8 pad_0x0[0x1470e];
    bool no_drop;  // 0x1470e
};

struct UnkInfesterBallActor_ovb0130 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

// fn_71005ce70c: the ball's death action: "death_nodrop" while its CInfesterBallAIComponent has the flag
// at +0x1470e set, else "death".
const CStrId* fn_71005ce70c(void* self, UnkInfesterBallActor_ovb0130* actor) {
    UnkReflObject* o = actor->ai;
    auto* ai = reinterpret_cast<UnkInfesterBallAI_ovb0130*>(
        o && fn_710009d50c(o->vf_0x0(), fn_71005ceda4()) ? o : nullptr);
    static const CStrId s_death("death", true);
    if (ai && ai->no_drop) {
        static const CStrId s_no_drop("death_nodrop", true);
        return &s_no_drop;
    }
    return &s_death;
}

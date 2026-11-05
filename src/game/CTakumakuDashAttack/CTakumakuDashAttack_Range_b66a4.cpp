// CCharClassTakumakuDashAttack's unit (tools/tu_map.py): the attack's range (+0x40). When it is
// positive and the actor's AI component (+0x240) is a CTakumakuAIComponent, -1 when the component's
// flag at +0x14758 is set, else the smaller of the range and the distance the component reports
// (fn_71010fc778). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_b34.cpp declares it.
bool fn_71010fc778(void* self, f32* out);

struct UnkTakumakuDashAttack_b66a4 {
    u8 pad_0x0[0x40];
    f32 range;  // 0x40
};

f32 fn_71006cdecc(UnkTakumakuDashAttack_b66a4* self, UnkActor* actor) {
    f32 range = self->range;
    if (range > 0.001f) {
        f32 dist = -1.0f;
        if (actor) {
            auto* ai = static_cast<base::reflection::UnkReflObject*>(actor->field_0x240);
            if (ai && fn_710009d50c(ai->vf_0x0(), fn_71006d3ba8())) {
                if (reinterpret_cast<u8*>(ai)[0x14758])
                    range = -1.0f;
                else if (fn_71010fc778(ai, &dist))
                    range = __builtin_fminf(range, dist);
            }
        }
    }
    return range;
}

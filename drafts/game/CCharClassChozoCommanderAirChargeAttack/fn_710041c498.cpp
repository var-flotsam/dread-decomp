// CCharClassChozoCommanderAirChargeAttack's unit: the attack's range (+0x40), or -1 when the actor's
// CChozoCommanderAIComponent puts it within half its reach plus 10 of the height it faces.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "types.h"

// As src/game/CAIComponent/CAIComponent_ReachRadius.cpp has it.
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkAirChargeCommander_l41c498 {
    u8 pad_0x0[0x15d1c];
    f32 field_0x15d1c[3];
};

struct UnkAirChargeAttack_l41c498 {
    u8 pad_0x0[0x40];
    f32 range;  // 0x40
};

f32 fn_710041c498(UnkAirChargeAttack_l41c498* self, UnkActor* actor) {
    f32 range = self->range;
    if (actor) {
        auto* component = static_cast<base::reflection::UnkReflObject*>(actor->field_0x240);
        if (component && fn_710009d50c(component->vf_0x0(), fn_710044e708())) {
            void* tracked = actor->field_0x1f8;
            if (tracked) {
                f32* heights = reinterpret_cast<UnkAirChargeCommander_l41c498*>(component)->field_0x15d1c;
                const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
                f32 x = actor->field_0x44;
                f32 dist = m.m[2] >= 0.0f ? x - heights[0] : heights[2] - x;
                UnkAIReach reach = fn_7100977b90(tracked);
                if (dist <= reach.radius * 0.5f + 10.0f)
                    range = -1.0f;
            }
        }
    }
    return range;
}

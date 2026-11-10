// CCharClassSclawkLifeComponent's unit (tools/tu_map.py unit 1004): fn_710067be80 picks the death animation
// id: "death", or, while the owner's animation (+0x220) plays 0x1cdf12aa84de383c, "death_init" when the
// track 0xf4061ff25a2163d6 is at most 5 and "death_end" past 18. The ids are function-local statics.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkSclawkOwner_ovb0136 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

const CStrId* fn_710067be80(void* self, UnkSclawkOwner_ovb0136* owner) {
    UnkAnimationComponent* anim = owner->field_0x220;
    f32 t = fn_7101177ea0(anim, 0xf4061ff25a2163d6);
    static CStrId s_death("death", true);
    if (fn_710117ae38(anim, 0x1cdf12aa84de383c, false)) {
        if (t <= 5.0f) {
            static CStrId s_init("death_init", true);
            return &s_init;
        }
        if (t > 18.0f) {
            static CStrId s_end("death_end", true);
            return &s_end;
        }
    }
    return &s_death;
}

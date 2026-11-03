// CAIComponent (tools/tu_map.py unit 2702): whether the owner's character class (the animation component
// at +0x220 of the owner at +0x30) allows graze tracking: it has a CCharClassAllowGrazeTrack and neither a
// CCharClassTurnTrack nor a CCharClassIgnoreGrazeTrack (fn_710117b348). Names and layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkAIComponent_b49a6 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

bool fn_71010fe668(UnkAIComponent_b49a6* self) {
    if (!self->owner)
        return false;
    auto* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!cls)
        return false;
    if (fn_710117b348(cls, fn_7100d2b3a8()))
        return false;
    if (fn_710117b348(cls, fn_7100c2eff8()))
        return false;
    return fn_710117b348(cls, fn_7100bfbb68());
}

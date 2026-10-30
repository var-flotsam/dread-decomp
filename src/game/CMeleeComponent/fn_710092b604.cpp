// In CMeleeComponent's unit (unit 1421 of tools/tu_map.py): whether the owner (+0x30) is playing a
// CCharClassMeleeTrack (fn_7100d1ab58; through its animation component at +0x220). Names
// and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkMeleeOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkMeleeComponent {
    u8 pad_0x0[0x30];
    UnkMeleeOwner* owner;
};

bool fn_710092b604(UnkMeleeComponent* self) {
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation)
            return fn_710117b348(animation, fn_7100d1ab58());
    }
    return false;
}

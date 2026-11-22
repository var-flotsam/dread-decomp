// In CMeleeComponent's unit (unit 1421 of tools/tu_map.py): whether the owner (+0x30) is playing a
// CCharClassMeleeTrack (fn_7100d1ab58; through its animation component at +0x220). Names
// and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CMeleeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkMeleeComponent.h"
#include "game/UnkMeleeOwner.h"

template <const base::reflection::CClass* (*Class)()>
static inline bool HasTrack(UnkMeleeComponent* self) {
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation)
            return fn_710117b348(animation, Class());
    }
    return false;
}

bool fn_710092b604(UnkMeleeComponent* self) { return HasTrack<fn_7100d1ab58>(self); }
// The same for CCharClassMeleeCameraAllowedTrack, next door.
bool fn_710092b648(UnkMeleeComponent* self) { return HasTrack<fn_7100d18d48>(self); }

// A CHangableGrappleSurfaceComponent check: true unless the actor's character class (+0x220 of the
// owner at +0x30) has a CCharClassCantBeGrappledTrack (fn_710117b348). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"

struct UnkHangableGrappleSurfaceComponent {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

bool fn_71008255f4(UnkHangableGrappleSurfaceComponent* self) {
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (cls == nullptr)
        return true;
    return !fn_710117b348(cls, fn_7100c0ee0c());
}

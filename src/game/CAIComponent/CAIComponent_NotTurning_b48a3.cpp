// CAIComponent unit (tools/tu_map.py): true when the owner (+0x30) has an animation component (+0x220)
// that is not playing a CCharClassTurnTrack; false without one. Names and layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTurnOwner_b48a3 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkTurnComponent_b48a3 {
    u8 pad_0x0[0x30];
    UnkTurnOwner_b48a3* owner;
};

bool fn_71010fe6d4(UnkTurnComponent_b48a3* self) {
    if (!self->owner)
        return false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    return !fn_710117b348(animation, fn_7100d2b3a8());
}

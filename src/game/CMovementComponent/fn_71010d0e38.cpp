// In the CMovementComponent unit (unit 2688 of tools/tu_map.py): whether the owner (+0x30) is playing
// a CCharClassFlyingTrack (through its animation component at +0x220). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkMovementOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkMovement_71010d0e38 {
    u8 pad_0x0[0x30];
    UnkMovementOwner* owner;
};

bool fn_71010d0e38(UnkMovement_71010d0e38* self) {
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation) {
            if (fn_710117b348(animation, fn_7100cf96a8()))
                return true;
        }
    }
    return false;
}

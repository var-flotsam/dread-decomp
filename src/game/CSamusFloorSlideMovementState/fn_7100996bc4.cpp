// From the CSamusFloorSlideMovementState unit: whether the actor's animation component (+0x220 of the
// object at +0x30) has a track of the class fn_7100cf7d58 gives. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkSlideActor {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkSlideState {
    u8 pad_0x0[0x30];
    UnkSlideActor* actor;
};

bool fn_7100996bc4(UnkSlideState* self) {
    UnkAnimationComponent* animation = self->actor->animation;
    if (animation && fn_710117b348(animation, fn_7100cf7d58()))
        return true;
    return false;
}

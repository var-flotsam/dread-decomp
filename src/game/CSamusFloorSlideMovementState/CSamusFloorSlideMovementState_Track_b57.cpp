// CSamusFloorSlideMovementState unit (tools/tu_map.py): unless `skip`, checks the owner's animation
// component for the tracks it holds (fn_710117b348 with the track classes' getters).
// In state 3 without the enlarge-collision track, virtual slot 38 decides between leaving through
// fn_710099db00 and going on; otherwise a tweak-fall track ends it. Then sends the component a hash by
// the slide track it has (45, 26 or neither) with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h) and
// moves to state 5. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CSamusGroundMovementState/CParams.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSamusGroundState.h"
#include "types.h"

void fn_71009947e0(UnkSamusGroundState* self, bool skip) {
    if (skip)
        return;
    UnkAnimationComponent* anim = self->owner->animation;
    bool falling = fn_710117b348(anim, fn_7100d2fa08());
    if (self->state == 3 && self->owner->animation && !fn_710117b348(self->owner->animation, fn_7100cf8778())) {
        if (self->vf_38(false, false, nullptr)) {
            fn_710099db00(self, false);
            return;
        }
    } else if (falling) {
        return;
    }
    u64 hash;
    if (fn_710117b348(anim, fn_7100c1a968()))
        hash = 0xf85faf4733567310;
    else if (fn_710117b348(anim, fn_7100c1a458()))
        hash = 0x2e63b85dc34d3423;
    else
        hash = 0x005732a6bc3abead;
    UnkSpitSend(anim, hash, 0xf4061ff25a2163d6);
    self->state = 5;
}

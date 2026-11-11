// CMorphBallMovement's unit: whether the ball may do the action. With a target (+0x128 and its +0x10)
// the angle between the vector at +0x154 and the y axis is taken; unless the base check fn_71010d1598
// holds it must lie between a quarter and half a turn. No floor-slide-45 track on the owner's
// animation component and neither of the player's states of fn_7100df5874 / fn_7100df5a4c. Names and
// layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkGroundPlayer_b52;
struct UnkPlayer;

struct UnkMorphBallOwner_38e8 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkMorphBallTarget_38e8 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkMorphBall_38e8 {
    u8 pad_0x0[0x30];
    UnkMorphBallOwner_38e8* owner;  // 0x30
    u8 pad_0x38[0x128 - 0x38];
    UnkMorphBallTarget_38e8* field_0x128;
    u8 pad_0x130[0x154 - 0x130];
    f32 field_0x154;
    f32 field_0x158;
    f32 field_0x15c;
};

bool fn_71010d1598(UnkGroundPlayer_b52*);
bool fn_7100df5874(UnkPlayer*);
bool fn_7100df5a4c(UnkGameManager* self);

bool fn_71009438e8(UnkMorphBall_38e8* self) {
    f32 angle = -1.0f;
    if (self->field_0x128 && self->field_0x128->field_0x10) {
        f32 dot = self->field_0x154 * 0.0f + self->field_0x158 + self->field_0x15c * 0.0f;
        f32 upper = !(dot >= 1.0f) ? dot : 1.0f;
        angle = acosf((dot <= -1.0f) ? -1.0f : upper);
    }
    if (!fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self))) {
        if (!(angle > 0.7853982f) || !(angle < 1.5707964f))
            return false;
    }
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100c2d0cc()))
        return false;
    if (fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)))
        return false;
    return !fn_7100df5a4c(g_unk_7101d47310);
}

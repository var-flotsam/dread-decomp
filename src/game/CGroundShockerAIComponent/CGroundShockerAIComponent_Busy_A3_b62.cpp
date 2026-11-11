// CGroundShockerAIComponent unit: fn_71005a4be8 answers whether the AI is busy: with an animation (+0x220 of
// the owner), a timer (+0x14738) above 0.001 or the animation playing class fn_7100c368d8; otherwise whether
// the frost-bite component (+0x2c8) is frozen (fn_71007f9f20) and not yet thawing (fn_71007f9fd0).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFrostbiteComponent;

struct UnkShockerOwner_b62a3 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0xa0];
    UnkFrostbiteComponent* frostbite;
};

struct UnkShockerAI_b62a3 {
    u8 pad_0x0[0x30];
    UnkShockerOwner_b62a3* owner;
    u8 pad_0x38[0x14738 - 0x38];
    f32 field_0x14738;
};

u32 fn_71005a4be8(UnkShockerAI_b62a3* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkFrostbiteComponent* frostbite = self->owner->frostbite;
    if (animation) {
        if (self->field_0x14738 > 0.001f || fn_710117b348(animation, fn_7100c368d8()))
            return true;
    }
    if (frostbite)
        return fn_71007f9f20(frostbite) && !fn_71007f9fd0(frostbite);
    return false;
}

// CCharClassPoisonFlyAIComponent's unit (tools/tu_map.py): fn_71006386cc picks a mode for an actor:
// while its frost-bite component (+0x2c8) is frozen (fn_71007f9f20) and not thawing (fn_71007f9fd0)
// the stored mode at +0x15c; otherwise 2 when the animation component (+0x220) plays
// CCharClassAllowCrazyTrack (fn_710117b348), else 1. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFrostbiteComponent;
bool fn_71007f9f20(void*);
bool fn_71007f9fd0(UnkFrostbiteComponent*);

struct UnkPoisonFlyOwner_ovb0053 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0xa0];
    UnkFrostbiteComponent* frostbite;
};

struct UnkPoisonFlyAI_ovb0053 {
    u8 pad_0x0[0x15c];
    s32 field_0x15c;
};

s32 fn_71006386cc(UnkPoisonFlyAI_ovb0053* self, UnkPoisonFlyOwner_ovb0053* owner) {
    UnkFrostbiteComponent* frostbite = owner->frostbite;
    UnkAnimationComponent* animation = owner->animation;
    if (frostbite && fn_71007f9f20(frostbite) && !fn_71007f9fd0(frostbite))
        return self->field_0x15c;
    s32 mode = 1;
    if (animation && fn_710117b348(animation, fn_7100ce9338()))
        mode = 2;
    return mode;
}

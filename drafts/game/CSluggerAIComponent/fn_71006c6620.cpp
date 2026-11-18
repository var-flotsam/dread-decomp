// CSluggerAIComponent unit (tools/tu_map.py): the base's answer (fn_7101110cb0), or else the owner's
// movement's (+0x218, fn_71010cab0c), or else whether the owner's animation (+0x220) plays a
// CCharClassBumpedTrack. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkMoveSelf_b0143;
bool fn_7101110cb0(UnkMoveSelf_b0143* self);

struct UnkSluggerOwner_l6c6620 {
    u8 pad_0x0[0x218];
    void* movement;
    UnkAnimationComponent* animation;
};

struct UnkSluggerAI_l6c6620 {
    u8 pad_0x0[0x30];
    UnkSluggerOwner_l6c6620* owner;
};

bool fn_71006c6620(UnkSluggerAI_l6c6620* self) {
    bool result = fn_7101110cb0(reinterpret_cast<UnkMoveSelf_b0143*>(self));
    void* movement = self->owner ? self->owner->movement : nullptr;
    if (movement && !result)
        result |= fn_71010cab0c(movement);
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    if (animation && !result)
        result |= fn_710117b348(animation, fn_7100cee4b8());
    return result;
}
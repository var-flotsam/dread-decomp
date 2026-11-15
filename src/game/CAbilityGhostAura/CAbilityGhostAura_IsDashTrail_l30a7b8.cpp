// CAbilityGhostAura unit: false while the state at +0x448 is 0 or 2, otherwise whether the owner actor's
// character class (+0x220 of the object at +0x30 of the one at +0xc0) is the dash-trail track class. Names
// and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkGhostAuraActor_l30a7b8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* character_class;
};

struct UnkGhostAuraOwner_l30a7b8 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_l30a7b8* actor;
};

struct UnkAbility;

struct UnkAbility_l30a7b8 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraOwner_l30a7b8* owner;
    u8 pad_0xc8[0x448 - 0xc8];
    s32 state;
};

bool fn_710030a7b8(UnkAbility* ability) {
    UnkAbility_l30a7b8* self = reinterpret_cast<UnkAbility_l30a7b8*>(ability);
    if (self->state == 0 || self->state == 2)
        return false;
    if (self->owner == nullptr || self->owner->actor == nullptr)
        return false;
    UnkAnimationComponent* character_class = self->owner->actor->character_class;
    if (character_class != nullptr && fn_710117b348(character_class, fn_7100cfde48()))
        return true;
    return false;
}

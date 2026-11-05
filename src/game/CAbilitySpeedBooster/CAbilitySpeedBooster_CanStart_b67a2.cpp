// CAbilitySpeedBooster unit (tools/tu_map.py, the name from reflection): false while the owner actor (+0x30 of
// the object at +0xc0) has something at +0x210, or unless the state at +0x3b8 is in mode 1; otherwise true unless
// the actor's character class (+0x220) is the one fn_7100c35498 names (fn_710117b348). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkSpeedBoosterActor_b67a2 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[8];
    UnkAnimationComponent* character_class;
};

struct UnkSpeedBoosterOwner_b67a2 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterActor_b67a2* actor;
};

struct UnkSpeedBoosterState_b67a2 {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkSpeedBooster_b67a2 {
    u8 pad_0x0[0xc0];
    UnkSpeedBoosterOwner_b67a2* owner;
    u8 pad_0xc8[0x3b8 - 0xc8];
    UnkSpeedBoosterState_b67a2* state;
};

bool fn_710033c2e4(UnkSpeedBooster_b67a2* self) {
    UnkSpeedBoosterActor_b67a2* actor = self->owner->actor;
    if (actor->field_0x210 != nullptr)
        return false;
    UnkSpeedBoosterState_b67a2* state = self->state;
    if (state == nullptr || state->mode != 1)
        return false;
    UnkAnimationComponent* character_class = actor->character_class;
    if (character_class != nullptr && fn_710117b348(character_class, fn_7100c35498()))
        return false;
    return true;
}

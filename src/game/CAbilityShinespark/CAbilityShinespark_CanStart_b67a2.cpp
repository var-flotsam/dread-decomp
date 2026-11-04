// CAbilityShinespark unit (tools/tu_map.py, the name from reflection): false while the owner actor (+0x30 of
// the object at +0xc0) has something at +0x210, or unless the state at +0x6b0 is in mode 1 or 2; otherwise true
// unless the actor's character class (+0x220) is the one fn_7100bfcfa8 names (fn_710117b348). Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkShinesparkActor_b67a2 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[8];
    UnkAnimationComponent* character_class;
};

struct UnkShinesparkOwner_b67a2 {
    u8 pad_0x0[0x30];
    UnkShinesparkActor_b67a2* actor;
};

struct UnkShinesparkState_b67a2 {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkShinespark_b67a2 {
    u8 pad_0x0[0xc0];
    UnkShinesparkOwner_b67a2* owner;
    u8 pad_0xc8[0x6b0 - 0xc8];
    UnkShinesparkState_b67a2* state;
};

bool fn_7100331218(UnkShinespark_b67a2* self) {
    UnkShinesparkActor_b67a2* actor = self->owner->actor;
    if (actor->field_0x210 != nullptr)
        return false;
    UnkShinesparkState_b67a2* state = self->state;
    if (state == nullptr || static_cast<u32>(state->mode - 1) > 1)
        return false;
    return !fn_710117b348(actor->character_class, fn_7100bfcfa8());
}

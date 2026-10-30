// In CFrozenComponent's unit (unit 1260 of tools/tu_map.py, the name from reflection): whether the
// component counts as frozen. Its state is the word at +0x10 of the object at +0x238 (5 without
// one): states 2 and 3 count, and state 4 does unless the owner's animation (the owner at +0x30,
// its animation component at +0x220) is playing a CCharClassIgnoreRecoveryFrozenTrack. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkFrozenState {
    u8 pad_0x0[0x10];
    u32 state;
};

struct UnkFrozenOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkFrozenComponent {
    u8 pad_0x0[0x30];
    UnkFrozenOwner* owner;
    u8 pad_0x38[0x200];
    UnkFrozenState* field_0x238;
};

// NON_MATCHING(minor): the original loads the owner in each arm of the state test and keeps the two
// "state 4" results apart; Clang merges them. The behaviour is the same.
bool fn_71007f9f20(void* component) {
    UnkFrozenComponent* self = static_cast<UnkFrozenComponent*>(component);
    u32 state = self->field_0x238 ? self->field_0x238->state : 5;
    UnkFrozenOwner* owner = self->owner;
    bool frozen;
    if (!owner) {
        frozen = state == 4;
    } else {
        frozen = state == 4;
        if (frozen) {
            if (UnkAnimationComponent* animation = owner->animation) {
                if (fn_710117b348(animation, fn_7100c33128()))
                    return false;
                frozen = true;
            }
        }
    }
    return frozen | ((state & ~1u) == 2);
}

// CAbilityEnergyWave's unit (the name from reflection): every ability of the owner's list (+0x40, count +0x48)
// other than `except` whose state (CAbility's, state 7 deferring to the ability at +0x20; as
// src/game/CAbility/CAbility_StateChecks.cpp) is 3 or more is told about `except` through vtable slot 0x138.
// Names and layouts are placeholders.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71003020f0(CAbilityComponent* self, CAbility* except) {
    u32 count = self->abilities.count;
    if (count == 0)
        return;
    CAbility** it = self->abilities.data;
    CAbility** end = it + count;
    for (; it != end; ++it) {
        CAbility* ability = *it;
        if (ability != except && (u32)ability->State() >= 3)
            ability->Virtual(0x138, except);
    }
}

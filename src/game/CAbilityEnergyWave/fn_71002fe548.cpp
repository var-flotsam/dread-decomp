// Runs fn_71002fe318, then calls virtual slot 0x118 on each child object of the array at +0x40.
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fe548(CAbilityComponent* self) {
    fn_71002fe318(self);
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x118);
}

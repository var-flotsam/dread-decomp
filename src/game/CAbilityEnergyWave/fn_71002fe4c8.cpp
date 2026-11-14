// Calls virtual slot 0x108 on each child object of the array at +0x40.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fe4c8(CAbilityComponent* self) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x108);
}

// Calls virtual slot 0x98 on each child object of the array at +0x40, passing the float `dt` through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_7100301810(CAbilityComponent* self, float dt) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x98, dt);
}

// Calls virtual slot 0x130 on each child object of the array at +0x40, passing `arg` through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_7100301750(CAbilityComponent* self, void* arg) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x130, arg);
}

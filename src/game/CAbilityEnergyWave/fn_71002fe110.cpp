// Calls virtual slot 0x60 on each child object of the array at +0x40, passing `arg` through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fe110(CAbilityComponent* self, void* arg) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x60, arg);
}

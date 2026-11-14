// Calls virtual slot 0xf8 on each child object of the array at +0x40, passing `arg` through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fdf5c(CAbilityComponent* self, void* arg) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0xf8, arg);
}

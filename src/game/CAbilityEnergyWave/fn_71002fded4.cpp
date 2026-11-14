// Calls virtual slot 0xf0 on each child object of the array at +0x40, passing six arguments through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fded4(CAbilityComponent* self, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6) {
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0xf0, a1, a2, a3, a4, a5, a6);
}

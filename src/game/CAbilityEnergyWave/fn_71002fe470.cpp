// Unless a global flag is set, calls virtual slot 0x100 on each child object of the array at +0x40.
#include "game/UnkGameManager.h"
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fe470(CAbilityComponent* self) {
    if (g_unk_7101d47310->field_0x29c0)
        return;
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x100);
}

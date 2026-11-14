// Runs fn_71010c4228, then calls virtual slot 0x38 on each child object of the array at +0x40.
#include "game/CComponent.h"
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002fd72c(CAbilityComponent* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x38);
}

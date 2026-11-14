// Runs fn_71010c424c, then calls virtual slot 0x50 on each child object of the array at +0x40, passing `dt` through.
#include "game/CComponent.h"
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_71002ffea8(CAbilityComponent* self, float dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x50, dt);
}

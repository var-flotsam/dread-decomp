// Runs fn_710024fb28, then calls virtual slot 0x58 on each child object of the array at +0x40, passing `dt` through.
#include "game/CAbilityLayout.h"
#include "types.h"

void fn_710024fb28(UnkBaseComponent*);  // the actor component base step

void fn_71002fff00(CAbilityComponent* self, float dt) {
    fn_710024fb28(self);
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x58, dt);
}

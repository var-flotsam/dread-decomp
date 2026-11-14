// Calls virtual slot 0x128 on each child object of the array at +0x40, collecting a result through an out parameter.
#include "game/CAbilityLayout.h"
#include "types.h"

u32 fn_71003017a0(CAbilityComponent* self, void* arg) {
    u32 result = 0;
    CAbility** end = self->end();
    for (CAbility** p = self->begin(); p != end; ++p)
        (*p)->Virtual(0x128, arg, &result);
    return result;
}

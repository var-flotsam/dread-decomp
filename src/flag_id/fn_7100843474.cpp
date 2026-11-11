// True when the first check passes and the second returns zero.
#include "game/CPrimaryGun.h"
#include "types.h"

s32 fn_71008434b8(void* self);

bool fn_7100843474(void* self) {
    return fn_7100857344(self) && fn_71008434b8(self) == 0;
}

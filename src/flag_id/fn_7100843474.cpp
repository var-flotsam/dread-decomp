// True when the first check passes and the second returns zero.
#include "game/CHyperBeamGun.h"
#include "game/CPrimaryGun.h"
#include "types.h"

bool fn_7100843474(void* self) {
    return fn_7100857344(self) && CHyperBeamGun_GetChargeBlockFlags(self) == 0;
}

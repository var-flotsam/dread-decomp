// CGroundShockerAttack (reflection: size 0x60): its copy assignment (fn_71005a1460) and move
// assignment (fn_71005a15bc), both the implicit copy assignment behind a self check.
#include "game/CAttack.h"
#include "game/CGroundShockerAttack.h"

CGroundShockerAttack* fn_71005a1460(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

CGroundShockerAttack* fn_71005a15bc(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

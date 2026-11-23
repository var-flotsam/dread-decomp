// CGroundShockerAttack (reflection: size 0x60): its copy constructor (fn_71005a12ec) and move
// constructor (fn_71005a13a0), both the implicit copy.
#include "game/CAttack.h"
#include "game/CGroundShockerAttack.h"

#include <new>

void CGroundShockerAttack::refl_CopyConstruct(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    new (self) CGroundShockerAttack(*other);
}

void CGroundShockerAttack::refl_MoveConstruct(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    new (self) CGroundShockerAttack(*other);
}

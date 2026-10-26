// CGroundShockerAttack (reflection: size 0x60): its copy constructor (fn_71005a12ec) and move
// constructor (fn_71005a13a0), both the implicit copy.
#include "game/CAttack.h"

#include <new>

struct CGroundShockerAttack : CAttack {
    ~CGroundShockerAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    UnkAttackListener field_0x40;
};

void fn_71005a12ec(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    new (self) CGroundShockerAttack(*other);
}

void fn_71005a13a0(CGroundShockerAttack* self, const CGroundShockerAttack* other) {
    new (self) CGroundShockerAttack(*other);
}

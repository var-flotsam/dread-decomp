// CSabotoruTurnInDoorAttack (reflection: size 0x60): its copy constructor (fn_7100673dd4) and move
// constructor (fn_7100673ea0), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CSabotoruTurnInDoorAttack : CAttack {
    ~CSabotoruTurnInDoorAttack() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
};

void fn_7100673dd4(CSabotoruTurnInDoorAttack* self, const CSabotoruTurnInDoorAttack* other) {
    new (self) CSabotoruTurnInDoorAttack(*other);
}

void fn_7100673ea0(CSabotoruTurnInDoorAttack* self, const CSabotoruTurnInDoorAttack* other) {
    new (self) CSabotoruTurnInDoorAttack(*other);
}

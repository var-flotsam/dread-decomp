// CInfesterShootAttack (reflection: size 0x60): its copy constructor (fn_71005c743c) and move
// constructor (fn_71005c7508), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CInfesterShootAttack : CAttack {
    ~CInfesterShootAttack() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
};

void fn_71005c743c(CInfesterShootAttack* self, const CInfesterShootAttack* other) {
    new (self) CInfesterShootAttack(*other);
}

void fn_71005c7508(CInfesterShootAttack* self, const CInfesterShootAttack* other) {
    new (self) CInfesterShootAttack(*other);
}

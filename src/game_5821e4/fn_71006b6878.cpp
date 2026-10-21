// CShelmitPlasmaRayAttack (reflection: size 0xb0): its copy constructor (fn_71006b6878) and
// move constructor (fn_71006b69b4), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CShelmitPlasmaRayAttack : CAttack {
    ~CShelmitPlasmaRayAttack() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    UnkAttackWords<13> field_0x78;
};

void fn_71006b6878(CShelmitPlasmaRayAttack* self, const CShelmitPlasmaRayAttack* other) {
    new (self) CShelmitPlasmaRayAttack(*other);
}

void fn_71006b69b4(CShelmitPlasmaRayAttack* self, const CShelmitPlasmaRayAttack* other) {
    new (self) CShelmitPlasmaRayAttack(*other);
}

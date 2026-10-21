// CShakernautDoubleGroundShockAttack (reflection: size 0x68): its copy constructor (fn_71006a5828)
// and move constructor (fn_71006a58f4), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CShakernautDoubleGroundShockAttack : CAttack {
    ~CShakernautDoubleGroundShockAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
    u32 field_0x60;
};

void fn_71006a5828(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    new (self) CShakernautDoubleGroundShockAttack(*other);
}

void fn_71006a58f4(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    new (self) CShakernautDoubleGroundShockAttack(*other);
}

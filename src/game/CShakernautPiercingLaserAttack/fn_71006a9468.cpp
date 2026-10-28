// CShakernautPiercingLaserAttack (reflection: size 0xc0): its copy constructor (fn_71006a9468)
// and move constructor (fn_71006a95bc), both the implicit copy.
#include "game/CAttack.h"

#include <new>

struct CShakernautPiercingLaserAttack : CAttack {
    ~CShakernautPiercingLaserAttack() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
    UnkAttackWords<13> field_0x74;
    u64 field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
};

void fn_71006a9468(CShakernautPiercingLaserAttack* self, const CShakernautPiercingLaserAttack* other) {
    new (self) CShakernautPiercingLaserAttack(*other);
}

void fn_71006a95bc(CShakernautPiercingLaserAttack* self, const CShakernautPiercingLaserAttack* other) {
    new (self) CShakernautPiercingLaserAttack(*other);
}

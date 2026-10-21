// CKraidBouncingCreaturesAttack (reflection: size 0x68, parent CKraidAttack): its copy constructor
// (fn_71005f3ad4) and move constructor (fn_71005f3ba8), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CKraidAttack : CAttack {
    ~CKraidAttack() override;
    u32 field_0x28;
    u32 field_0x2c;
};

struct CKraidBouncingCreaturesAttack : CKraidAttack {
    ~CKraidBouncingCreaturesAttack() override;
    u64 field_0x30;
    u64 field_0x38;
    u32 field_0x40;
    bool field_0x44;
    UnkAttackListener field_0x48;
};

void fn_71005f3ad4(CKraidBouncingCreaturesAttack* self, const CKraidBouncingCreaturesAttack* other) {
    new (self) CKraidBouncingCreaturesAttack(*other);
}

void fn_71005f3ba8(CKraidBouncingCreaturesAttack* self, const CKraidBouncingCreaturesAttack* other) {
    new (self) CKraidBouncingCreaturesAttack(*other);
}

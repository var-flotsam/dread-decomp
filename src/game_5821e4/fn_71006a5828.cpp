// CShakernautDoubleGroundShockAttack (reflection: size 0x68): its copy constructor (fn_71006a5828)
// and move constructor (fn_71006a58f4), both the implicit copy,
// and its copy (fn_71006a59cc) and move (fn_71006a5b3c) assignments, both the implicit copy assignment.
#include "CAttack.h"

#include <new>

struct CShakernautDoubleGroundShockAttack : CAttack {
    ~CShakernautDoubleGroundShockAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
    u32 field_0x60;

    __attribute__((always_inline)) CShakernautDoubleGroundShockAttack& operator=(const CShakernautDoubleGroundShockAttack&) = default;
};

void fn_71006a5828(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    new (self) CShakernautDoubleGroundShockAttack(*other);
}

void fn_71006a58f4(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    new (self) CShakernautDoubleGroundShockAttack(*other);
}

CShakernautDoubleGroundShockAttack* fn_71006a59cc(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

CShakernautDoubleGroundShockAttack* fn_71006a5b3c(CShakernautDoubleGroundShockAttack* self, const CShakernautDoubleGroundShockAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

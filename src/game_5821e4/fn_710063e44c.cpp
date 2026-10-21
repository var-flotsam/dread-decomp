// CQuetzoaXMultiTargetAttack (reflection: size 0x60): its copy constructor (fn_710063e44c) and move
// constructor (fn_710063e500), both the implicit copy.
#include "CAttack.h"

#include <new>

struct CQuetzoaXMultiTargetAttack : CAttack {
    ~CQuetzoaXMultiTargetAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    UnkAttackListener field_0x40;
};

void fn_710063e44c(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    new (self) CQuetzoaXMultiTargetAttack(*other);
}

void fn_710063e500(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    new (self) CQuetzoaXMultiTargetAttack(*other);
}

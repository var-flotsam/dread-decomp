// CQuetzoaXMultiTargetAttack (reflection: size 0x60): its copy constructor (fn_710063e44c) and move
// constructor (fn_710063e500), both the implicit copy,
// and its copy (fn_710063e5c0) and move (fn_710063e71c) assignments, both the implicit copy assignment.
#include "game/CAttack.h"

#include <new>

struct CQuetzoaXMultiTargetAttack : CAttack {
    ~CQuetzoaXMultiTargetAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    UnkAttackListener field_0x40;

    __attribute__((always_inline)) CQuetzoaXMultiTargetAttack& operator=(const CQuetzoaXMultiTargetAttack&) = default;
};

void fn_710063e44c(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    new (self) CQuetzoaXMultiTargetAttack(*other);
}

void fn_710063e500(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    new (self) CQuetzoaXMultiTargetAttack(*other);
}

CQuetzoaXMultiTargetAttack* fn_710063e5c0(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

CQuetzoaXMultiTargetAttack* fn_710063e71c(CQuetzoaXMultiTargetAttack* self, const CQuetzoaXMultiTargetAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

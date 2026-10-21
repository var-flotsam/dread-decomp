// CTakumakuDashAttack (reflection: size 0x78): its copy constructor (fn_71006d0908) and move
// constructor (fn_71006d09d8), both the implicit copy,
// and its copy (fn_71006d0ab4) and move (fn_71006d0c2c) assignments, both the implicit copy assignment.
#include "CAttack.h"

#include <new>

struct CTakumakuDashAttack : CAttack {
    ~CTakumakuDashAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    UnkAttackListener field_0x40;
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
    bool field_0x74;
    bool field_0x75;
    bool field_0x76;

    __attribute__((always_inline)) CTakumakuDashAttack& operator=(const CTakumakuDashAttack&) = default;
};

void fn_71006d0908(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    new (self) CTakumakuDashAttack(*other);
}

void fn_71006d09d8(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    new (self) CTakumakuDashAttack(*other);
}

CTakumakuDashAttack* fn_71006d0ab4(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

CTakumakuDashAttack* fn_71006d0c2c(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    if (self != other)
        *self = *other;
    return self;
}

// CTakumakuDashAttack (reflection: size 0x78): its copy constructor (fn_71006d0908) and move
// constructor (fn_71006d09d8), both the implicit copy.
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
};

void fn_71006d0908(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    new (self) CTakumakuDashAttack(*other);
}

void fn_71006d09d8(CTakumakuDashAttack* self, const CTakumakuDashAttack* other) {
    new (self) CTakumakuDashAttack(*other);
}

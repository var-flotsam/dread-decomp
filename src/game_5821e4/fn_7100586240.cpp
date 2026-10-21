// CGoliathXBurstProjectionBombsAttack (reflection: size 0x90): its copy constructor
// (fn_7100586240) and move constructor (fn_7100586310), both the implicit copy.
#include "CAttack.h"

#include <new>

// A 0x28-byte member copied out of line (fn_7100241a1c).
struct UnkMember_7100241a1c {
    u64 unk_0x0[5];
    UnkMember_7100241a1c(const UnkMember_7100241a1c& other);
};

struct CGoliathXBurstProjectionBombsAttack : CAttack {
    ~CGoliathXBurstProjectionBombsAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    UnkMember_7100241a1c field_0x38;
    u64 field_0x60;
    u32 field_0x68;
    UnkAttackListener field_0x70;
};

void fn_7100586240(CGoliathXBurstProjectionBombsAttack* self,
                   const CGoliathXBurstProjectionBombsAttack* other) {
    new (self) CGoliathXBurstProjectionBombsAttack(*other);
}

void fn_7100586310(CGoliathXBurstProjectionBombsAttack* self,
                   const CGoliathXBurstProjectionBombsAttack* other) {
    new (self) CGoliathXBurstProjectionBombsAttack(*other);
}

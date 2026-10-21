// CGoliathAttack (reflection: size 0x70): its copy constructor (fn_710058f3d4) and move
// constructor (fn_710058f4cc), both the implicit copy.
#include "CAttack.h"
#include "base/global/CStrId.h"

#include <new>

struct CGoliathAttack : CAttack {
    ~CGoliathAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    base::global::CStrId field_0x40;
    UnkAttackListener field_0x48;
    u64 field_0x68;
};

void fn_710058f3d4(CGoliathAttack* self, const CGoliathAttack* other) {
    new (self) CGoliathAttack(*other);
}

void fn_710058f4cc(CGoliathAttack* self, const CGoliathAttack* other) {
    new (self) CGoliathAttack(*other);
}

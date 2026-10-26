// CGoliathXSlamAttack (reflection: size 0x80, parent CGoliathAttack): its copy constructor
// (fn_71005873d0) and move constructor (fn_71005874e8), both the implicit copy.
#include "game/CAttack.h"
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

struct CGoliathXSlamAttack : CGoliathAttack {
    ~CGoliathXSlamAttack() override;
    u64 field_0x70;
    bool field_0x78;
};

void fn_71005873d0(CGoliathXSlamAttack* self, const CGoliathXSlamAttack* other) {
    new (self) CGoliathXSlamAttack(*other);
}

void fn_71005874e8(CGoliathXSlamAttack* self, const CGoliathXSlamAttack* other) {
    new (self) CGoliathXSlamAttack(*other);
}

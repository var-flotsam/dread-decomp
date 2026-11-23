// CGoliathXSlamAttack (reflection: size 0x80, parent CGoliathAttack): its copy constructor
// (fn_71005873d0) and move constructor (fn_71005874e8), both the implicit copy.
#include "game/CAttack.h"
#include "base/global/CStrId.h"
#include "game/CGoliathAttack.h"

#include <new>

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

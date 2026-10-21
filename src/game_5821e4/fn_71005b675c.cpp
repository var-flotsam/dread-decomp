// CHydrogigaPolypsAttack (reflection: size 0x78, parent CHydrogigaAttack): its copy
// constructor (fn_71005b675c) and move constructor (fn_71005b68c0), both the implicit copy.
#include "CAttack.h"
#include "base/UnkCloneRecords.h"

#include <new>

struct CHydrogigaAttack : CAttack {
    ~CHydrogigaAttack() override;
    base::UnkStrIdVec field_0x28;
    u64 field_0x50;
    u64 field_0x58;
};

struct CHydrogigaPolypsAttack : CHydrogigaAttack {
    ~CHydrogigaPolypsAttack() override;
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
};

void fn_71005b675c(CHydrogigaPolypsAttack* self, const CHydrogigaPolypsAttack* other) {
    new (self) CHydrogigaPolypsAttack(*other);
}

void fn_71005b68c0(CHydrogigaPolypsAttack* self, const CHydrogigaPolypsAttack* other) {
    new (self) CHydrogigaPolypsAttack(*other);
}

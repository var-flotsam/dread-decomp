// CHydrogigaAttack (reflection: size 0x60): its copy constructor (fn_71005b32d0) and move
// constructor (fn_71005b3414), both the implicit copy.
#include "CAttack.h"
#include "base/UnkCloneRecords.h"

#include <new>

struct CHydrogigaAttack : CAttack {
    ~CHydrogigaAttack() override;
    base::UnkStrIdVec field_0x28;
    u64 field_0x50;
    u64 field_0x58;
};

void fn_71005b32d0(CHydrogigaAttack* self, const CHydrogigaAttack* other) {
    new (self) CHydrogigaAttack(*other);
}

void fn_71005b3414(CHydrogigaAttack* self, const CHydrogigaAttack* other) {
    new (self) CHydrogigaAttack(*other);
}

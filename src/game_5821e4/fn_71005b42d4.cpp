// CHydrogigaBraidAttack (reflection: size 0x90, parent CHydrogigaAttack): its copy constructor,
// out of line; the descriptor's copy and move constructors (fn_71005b4260, fn_71005b4264)
// branch to it.
#include "CAttack.h"
#include "base/UnkCloneRecords.h"

#include <new>

struct CHydrogigaAttack : CAttack {
    ~CHydrogigaAttack() override;
    base::UnkStrIdVec field_0x28;
    u64 field_0x50;
    u64 field_0x58;
};

struct CHydrogigaBraidAttack : CHydrogigaAttack {
    ~CHydrogigaBraidAttack() override;
    __attribute__((always_inline)) CHydrogigaBraidAttack(const CHydrogigaBraidAttack&) = default;
    u64 field_0x60;
    bool field_0x68;
    UnkAttackListener field_0x70;
};

void fn_71005b42d4(CHydrogigaBraidAttack* self, const CHydrogigaBraidAttack* other) {
    new (self) CHydrogigaBraidAttack(*other);
}

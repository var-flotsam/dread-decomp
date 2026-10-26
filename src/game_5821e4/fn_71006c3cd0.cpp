// CSluggerSpitAttack (reflection: size 0xe8): its copy constructor, out of line; the
// descriptor's copy and move constructors (fn_71006c3c5c, fn_71006c3c60) branch to it.
#include "game/CAttack.h"

#include <new>

// A 0x48-byte member copied out of line (fn_7100331c7c).
struct UnkMember_7100331c7c {
    u64 unk_0x0[9];
    UnkMember_7100331c7c(const UnkMember_7100331c7c& other);
};

struct CSluggerSpitAttack : CAttack {
    ~CSluggerSpitAttack() override;
    __attribute__((always_inline)) CSluggerSpitAttack(const CSluggerSpitAttack&) = default;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    UnkAttackWords<10> field_0x58;
    bool field_0x80;
    UnkMember_7100331c7c field_0x88;
    u64 field_0xd0;
    u64 field_0xd8;
    UnkAttackRefPtr field_0xe0;
};

void fn_71006c3cd0(CSluggerSpitAttack* self, const CSluggerSpitAttack* other) {
    new (self) CSluggerSpitAttack(*other);
}

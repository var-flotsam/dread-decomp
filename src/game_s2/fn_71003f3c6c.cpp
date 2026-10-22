// CBigkranXSpitAttack (reflection: size 0x128): its copy constructor, out of line.
#include "../game_5821e4/CAttack.h"

// A 0x48-byte member copied out of line (fn_7100331c7c).
struct UnkMember_7100331c7c {
    u64 unk_0x0[9];
    UnkMember_7100331c7c(const UnkMember_7100331c7c& other);
};

// A 0x28-byte growable array copied out of line (fn_71000cf1fc).
struct UnkMember_71000cf1fc {
    u64 unk_0x0[5];
    UnkMember_71000cf1fc(const UnkMember_71000cf1fc& other);
};

// Copied whole, padding included.
struct UnkWord12 {
    u64 field_0x0;
    u32 field_0x8;
};

// Copied as one 0x1c-byte block.
struct UnkF32x7 {
    f32 v[7];
};

struct CBigkranXSpitAttack : CAttack {
    ~CBigkranXSpitAttack() override;
    __attribute__((always_inline)) CBigkranXSpitAttack(const CBigkranXSpitAttack&) = default;
    UnkWord12 field_0x28;
    UnkAttackListener field_0x38;
    UnkAttackWords<10> field_0x58;
    bool field_0x80;
    UnkMember_7100331c7c field_0x88;
    UnkF32x7 field_0xd0;
    UnkAttackWords<3> field_0xec;
    UnkAttackRefPtr field_0xf8;
    UnkMember_71000cf1fc field_0x100;
};
static_assert(sizeof(CBigkranXSpitAttack) == 0x128, "CBigkranXSpitAttack layout");

void fn_71003f3c6c(CBigkranXSpitAttack* self, const CBigkranXSpitAttack* other) {
    new (self) CBigkranXSpitAttack(*other);
}

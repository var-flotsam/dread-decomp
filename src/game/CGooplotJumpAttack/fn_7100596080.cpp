// CGooplotJumpAttack (reflection: size 0x88): its copy constructor (fn_7100596080) and move
// constructor (fn_710059618c), both the implicit copy.
#include "game/CAttack.h"

#include <new>

struct CGooplotJumpAttack : CAttack {
    ~CGooplotJumpAttack() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener field_0x38;
    u64 field_0x58;
    bool field_0x60;
    UnkAttackWords<6> field_0x64;
    u32 field_0x7c;
    u32 field_0x80;
};

void fn_7100596080(CGooplotJumpAttack* self, const CGooplotJumpAttack* other) {
    new (self) CGooplotJumpAttack(*other);
}

void fn_710059618c(CGooplotJumpAttack* self, const CGooplotJumpAttack* other) {
    new (self) CGooplotJumpAttack(*other);
}

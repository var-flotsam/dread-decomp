// CGooplotUndoJumpAttack (reflection: size 0x88, parent CGooplotJumpAttack, no fields of its
// own): its copy constructor (fn_7100596f40) and move constructor (fn_710059705c), both the
// implicit copy.
#include "CAttack.h"

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

struct CGooplotUndoJumpAttack : CGooplotJumpAttack {
    ~CGooplotUndoJumpAttack() override;
};

void fn_7100596f40(CGooplotUndoJumpAttack* self, const CGooplotUndoJumpAttack* other) {
    new (self) CGooplotUndoJumpAttack(*other);
}

void fn_710059705c(CGooplotUndoJumpAttack* self, const CGooplotUndoJumpAttack* other) {
    new (self) CGooplotUndoJumpAttack(*other);
}

// CKraidFierceSwipeAttack (reflection: size 0x58): its move assignment (fn_71005f4bb8). It copies the plain
// words first and tests for self-assignment only before sharing the listener owner's weak handle.
// Near copy of fn_71006338dc (src/game/CPoisonFlyDiveAttack/CPoisonFlyDiveAttack.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkBlock32_71005f4bb8 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_71005f4bb8 {
    u64 field_0x0;
    u64 field_0x8;
};

struct CKraidFierceSwipeAttack_View {
    void** vtable;
    UnkBlock32_71005f4bb8 field_0x8;
    u64 field_0x28;
    u64 field_0x30;
    void** listener_vtable;
    UnkWords_71005f4bb8 field_0x40;
    UnkWeakHandle* field_0x50;
};

CKraidFierceSwipeAttack_View* fn_71005f4bb8(CKraidFierceSwipeAttack_View* self, const CKraidFierceSwipeAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x40 = other->field_0x40;
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    return self;
}

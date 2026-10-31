// CKraidSpinningNailsAttack (reflection: size 0x68): its move assignment (fn_71005f77f8). It copies the plain
// words, shares the listener owner's weak handle unless assigned to itself, then copies the
// trailing words. Near copy of fn_71006a5b3c
// (src/game/CShakernautDoubleGroundShockAttack/fn_71006a5828.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkBlock32_71005f77f8 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_71005f77f8 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkBlock24_71005f77f8 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct CKraidSpinningNailsAttack_View {
    void** vtable;
    UnkBlock32_71005f77f8 field_0x8;
    u64 field_0x28;
    void** listener_vtable;
    UnkWords_71005f77f8 field_0x38;
    UnkWeakHandle* field_0x48;
    UnkBlock24_71005f77f8 field_0x50;
};

CKraidSpinningNailsAttack_View* fn_71005f77f8(CKraidSpinningNailsAttack_View* self, const CKraidSpinningNailsAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x38 = other->field_0x38;
    if (self != other)
        CopyWeakRef(&self->field_0x48, other->field_0x48);
    self->field_0x50 = other->field_0x50;
    return self;
}

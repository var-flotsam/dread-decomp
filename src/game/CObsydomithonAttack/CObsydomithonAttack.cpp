// CObsydomithonAttack (reflection: size 0x68): its move assignment (fn_710062ac28). It copies the plain
// words, shares the listener owner's weak handle unless assigned to itself, then copies the
// trailing words. Near copy of fn_71006a5b3c
// (src/game/CShakernautDoubleGroundShockAttack/fn_71006a5828.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkBlock32_710062ac28 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_710062ac28 {
    u64 field_0x0;
    u64 field_0x8;
};

struct CObsydomithonAttack_View {
    void** vtable;
    UnkBlock32_710062ac28 field_0x8;
    u64 field_0x28;
    void** listener_vtable;
    UnkWords_710062ac28 field_0x38;
    UnkWeakHandle* field_0x48;
    UnkWords_710062ac28 field_0x50;
    u16 field_0x60;
};

CObsydomithonAttack_View* fn_710062ac28(CObsydomithonAttack_View* self, const CObsydomithonAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x38 = other->field_0x38;
    if (self != other)
        CopyWeakRef(&self->field_0x48, other->field_0x48);
    u64 field_0x50 = other->field_0x50.field_0x0;
    u64 field_0x58 = other->field_0x50.field_0x8;
    self->field_0x60 = other->field_0x60;
    self->field_0x50.field_0x0 = field_0x50;
    self->field_0x50.field_0x8 = field_0x58;
    return self;
}

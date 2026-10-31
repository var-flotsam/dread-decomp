// CHydrogigaBraidAttack: its move assignment (fn_71005b4494). It copies the plain words, assigns the member
// at +0x28 through fn_71000ce19c, and (unless assigned to itself) shares the weak handle of what
// the other one's +0x88 refers to. Near copy of fn_710058644c (src/game/CGoliathXBurstProjectionBombsAttack/), itself a copy of fn_71004963fc
// (src/game/CChozoWarriorGlaiveWalljumpAttack/fn_71004963fc.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkStruct_0x28 {
    u8 unk_0x0[0x28];
};

void fn_71000ce19c(UnkStruct_0x28* self, const UnkStruct_0x28* other);

struct UnkBlock32_71005b4494 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_71005b4494 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkTriple_71005b4494 {
    float x;
    float y;
    float z;
};

struct CHydrogigaBraidAttack_View {
    void** vtable;
    UnkBlock32_71005b4494 field_0x8;
    UnkStruct_0x28 field_0x28;
    UnkWords_71005b4494 field_0x50;
    u64 field_0x60;
    u8 field_0x68;
    u8 pad_0x69[3];
    u8 pad_0x6c[0xc];
    UnkWords_71005b4494 field_0x78;
    UnkWeakHandle* field_0x88;
};

CHydrogigaBraidAttack_View* fn_71005b4494(CHydrogigaBraidAttack_View* self, const CHydrogigaBraidAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    fn_71000ce19c(&self->field_0x28, &other->field_0x28);
    self->field_0x50 = other->field_0x50;
    u8 field_0x68 = other->field_0x68;
    self->field_0x60 = other->field_0x60;
    self->field_0x68 = field_0x68;
    self->field_0x78 = other->field_0x78;
    if (self != other)
        CopyWeakRef(&self->field_0x88, other->field_0x88);
    return self;
}

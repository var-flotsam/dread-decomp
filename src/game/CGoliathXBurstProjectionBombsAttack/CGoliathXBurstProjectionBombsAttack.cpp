// CGoliathXBurstProjectionBombsAttack: its move assignment (fn_710058644c). It copies the plain words, assigns the member
// at +0x38 through fn_7100241c78, and (unless assigned to itself) shares the weak handle of what
// the other one's +0x88 refers to. Near copy of fn_71004963fc
// (src/game/CChozoWarriorGlaiveWalljumpAttack/fn_71004963fc.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkMember_710058644c {
    u8 unk_0x0[0x28];
};

void fn_7100241c78(UnkMember_710058644c* self, const UnkMember_710058644c* other);

struct UnkBlock32_710058644c {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_710058644c {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkTriple_710058644c {
    float x;
    float y;
    float z;
};

struct CGoliathXBurstProjectionBombsAttack_View {
    void** vtable;
    UnkBlock32_710058644c field_0x8;
    UnkWords_710058644c field_0x28;
    UnkMember_710058644c field_0x38;
    u64 field_0x60;
    u32 field_0x68;
    u8 pad_0x6c[0xc];
    UnkWords_710058644c field_0x78;
    UnkWeakHandle* field_0x88;
};

CGoliathXBurstProjectionBombsAttack_View* fn_710058644c(CGoliathXBurstProjectionBombsAttack_View* self, const CGoliathXBurstProjectionBombsAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    fn_7100241c78(&self->field_0x38, &other->field_0x38);
    u32 field_0x68 = other->field_0x68;
    self->field_0x60 = other->field_0x60;
    self->field_0x68 = field_0x68;
    self->field_0x78 = other->field_0x78;
    if (self != other)
        CopyWeakRef(&self->field_0x88, other->field_0x88);
    return self;
}

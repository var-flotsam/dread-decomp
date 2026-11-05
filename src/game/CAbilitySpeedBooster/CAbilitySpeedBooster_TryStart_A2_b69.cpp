// CAbilitySpeedBooster unit (tools/tu_map.py, the name from reflection): unless the ability is already in state 6
// (CAbility's state, 7 deferring to the ability at +0x20), starts it (fn_71002fa070) when the owner actor has no
// character class (+0x220) or the charge (+0x398) has reached the threshold: the list value at +0x5a8
// (fn_7100030dec) when the list is not empty, else the tunable's +0x2c (g_unk_71726b0fc0).
// Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

struct UnkFloatList_b46a1 {
    void* data;
    u32 count;
};
f32 fn_7100030dec(UnkFloatList_b46a1* list);

struct UnkSmAbility3_A2_b63;
void fn_71002fa070(UnkSmAbility3_A2_b63*);

extern base::reflection::UnkTunable g_unk_71726b0fc0;

struct UnkSpeedBoosterActor_A2_b69 {
    u8 pad_0x0[0x220];
    void* character_class;  // +0x220
};

struct UnkSpeedBoosterOwner_A2_b69 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterActor_A2_b69* actor;  // +0x30
};

struct UnkSpeedBoosterState_A2_b69 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkSpeedBooster_A2_b69 {
    u8 pad_0x0[0x20];
    UnkSpeedBoosterState_A2_b69* field_0x20;
    u8 pad_0x28[0x8c];
    s32 field_0xb4;
    u8 pad_0xb8[0x8];
    UnkSpeedBoosterOwner_A2_b69* owner;  // +0xc0
    u8 pad_0xc8[0x398 - 0xc8];
    f32 charge;  // +0x398
    u8 pad_0x39c[0x5a8 - 0x39c];
    UnkFloatList_b46a1 thresholds;  // +0x5a8

    s32 State() const {
        s32 state = field_0xb4;
        if (state == 7 && field_0x20)
            state = field_0x20->state;
        return state;
    }
};

void fn_710033f6fc(UnkSpeedBooster_A2_b69* self) {
    if (self->State() == 6)
        return;
    UnkSpeedBoosterActor_A2_b69* actor = self->owner->actor;
    if (actor && actor->character_class) {
        f32 threshold = self->thresholds.count != 0
                            ? fn_7100030dec(&self->thresholds)
                            : *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726b0fc0) + 0x2c);
        if (!(self->charge >= threshold))
            return;
    }
    fn_71002fa070(reinterpret_cast<UnkSmAbility3_A2_b63*>(self));
}

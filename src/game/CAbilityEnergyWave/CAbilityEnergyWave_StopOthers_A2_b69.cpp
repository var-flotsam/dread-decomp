// CAbilityEnergyWave's unit (the name from reflection): every ability of the owner's list (+0x40, count +0x48)
// other than `except` whose state (CAbility's, state 7 deferring to the ability at +0x20; as
// src/game/CAbility/CAbility_StateChecks.cpp) is 3 or more is told about `except` through vtable slot 0x138.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAbilityState_A2_b69 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkAbility_A2_b69 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkAbilityState_A2_b69* field_0x20;
    u8 pad_0x28[0x8c];
    s32 field_0xb4;

    s32 State() const {
        s32 state = field_0xb4;
        if (state == 7 && field_0x20)
            state = field_0x20->field_0x10;
        return state;
    }
    void vf_0x138(UnkAbility_A2_b69* other) {
        reinterpret_cast<void (*)(UnkAbility_A2_b69*, UnkAbility_A2_b69*)>(vtable[0x138 / 8])(this, other);
    }
};

struct UnkAbilityOwner_A2_b69 {
    u8 pad_0x0[0x40];
    UnkAbility_A2_b69** abilities;  // +0x40
    u32 count;                      // +0x48
};

void fn_71003020f0(UnkAbilityOwner_A2_b69* self, UnkAbility_A2_b69* except) {
    u32 count = self->count;
    if (count == 0)
        return;
    UnkAbility_A2_b69** it = self->abilities;
    UnkAbility_A2_b69** end = it + count;
    for (; it != end; ++it) {
        UnkAbility_A2_b69* ability = *it;
        if (ability != except && (u32)ability->State() >= 3)
            ability->vf_0x138(except);
    }
}

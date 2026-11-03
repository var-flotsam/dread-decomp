// CAbility and its subclasses (CAbilityShinespark, CAbilitySonar, CAbilitySpeedBooster): tests of
// the ability's state at 0xb4, where state 7 defers to the state of the ability at 0x20 (and stays
// 7 without one). The states' meanings are unknown; names are placeholders.
#include "types.h"

struct UnkAbilityState {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkAbility {
    u8 pad_0x0[0x20];
    UnkAbilityState* field_0x20;
    u8 pad_0x28[0x8c];
    s32 field_0xb4;

    s32 State() const {
        s32 state = field_0xb4;
        if (state == 7 && field_0x20)
            state = field_0x20->field_0x10;
        return state;
    }
};

struct UnkAbilitySpeedBooster : UnkAbility {
    u8 pad_0xb8[0x394 - 0xb8];
    bool field_0x394;
};
static_assert(__builtin_offsetof(UnkAbilitySpeedBooster, field_0x394) == 0x394, "layout");

// CAbility (unit of 0x71002f8da8 and 0x71002fa550).
bool fn_71002f8da8(const UnkAbility* self) {
    s32 state = self->State();
    return state == 5 || state == 6;
}
bool fn_71002fa550(const UnkAbility* self) { return self->State() == 4; }
bool fn_71002fa584(const UnkAbility* self) {
    s32 state = self->State();
    return state == 5 || state == 6;
}
bool fn_71002fa5c0(const UnkAbility* self) { return self->State() == 6; }

// CAbilityShinespark.
bool fn_710032d650(const UnkAbility* self) { return self->State() == 6; }

// CAbilitySonar.
bool fn_7100336858(const UnkAbility* self) { return self->State() == 4; }
bool fn_710033688c(const UnkAbility* self) { return self->State() == 5; }

// CAbilitySpeedBooster.
bool fn_710033c4cc(const UnkAbility* self) { return self->State() == 6; }
bool fn_710033f074(const UnkAbilitySpeedBooster* self) {
    if (self->State() == 6)
        return false;
    return self->field_0x394;
}

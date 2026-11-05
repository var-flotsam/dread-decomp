// CAbilityEnergyWave's unit (the name from reflection): a reflected getter (the first parameter is unused) that
// answers, as a bool value, whether any ability of the object's list (+0x40, count +0x48) has a state (CAbility's,
// state 7 deferring to the ability at +0x20) of 2 or more. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkAbilityState_A2b_b69 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkAbility_A2b_b69 {
    u8 pad_0x0[0x20];
    UnkAbilityState_A2b_b69* field_0x20;
    u8 pad_0x28[0x8c];
    s32 field_0xb4;

    s32 State() const {
        s32 state = field_0xb4;
        if (state == 7 && field_0x20)
            state = field_0x20->field_0x10;
        return state;
    }
};

struct UnkAbilityOwner_A2b_b69 {
    u8 pad_0x0[0x40];
    UnkAbility_A2b_b69** abilities;  // +0x40
    u32 count;                       // +0x48

    bool AnyActive() const {
        for (UnkAbility_A2b_b69** it = abilities; it != abilities + count; ++it)
            if ((u32)(*it)->State() >= 2)
                return true;
        return false;
    }
};

UnkReflValue fn_7100302aa4(void*, char* obj) {
    UnkAbilityOwner_A2b_b69* owner = *reinterpret_cast<UnkAbilityOwner_A2b_b69**>(obj + 8);
    bool value = owner->AnyActive();
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

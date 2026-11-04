// CAbilitySonar unit (tools/tu_map.py, the name from reflection): when the given ability's id (*(+0xc8)) is
// "EnergyWave" (fn_7100304140) and the sonar is in state 5 (the word at +0xb4, or for state 7 the mode at +0x10
// of the object at +0x20), calls its own virtual at vtable +0xa0 with 1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

const base::global::CStrId& fn_7100304140();

struct UnkSonarAbility_b67a2 {
    u8 pad_0x0[0xc8];
    base::global::CStrId* id;
};

struct UnkSonarState_b67a2 {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkSonar_b67a2 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkSonarState_b67a2* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;

    void vf_0xa0(bool on) { reinterpret_cast<void (*)(UnkSonar_b67a2*, bool)>(vtable[0xa0 / 8])(this, on); }
};

void fn_71003368fc(UnkSonar_b67a2* self, UnkSonarAbility_b67a2* ability) {
    if (ability == nullptr)
        return;
    base::global::CStrId* id = ability->id;
    const base::global::CStrId& energy_wave = fn_7100304140();
    if (id->data != energy_wave.data)
        return;
    s32 state = self->state;
    if (state == 7) {
        if (self->field_0x20 == nullptr)
            return;
        state = self->field_0x20->mode;
    }
    if (state == 5)
        self->vf_0xa0(true);
}

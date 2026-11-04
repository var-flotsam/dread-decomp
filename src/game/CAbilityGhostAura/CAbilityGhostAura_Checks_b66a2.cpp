// CAbilityGhostAura unit (tools/tu_map.py, the name from reflection): in state 4 (+0xb4, or the state at
// +0x10 of the object at +0x20 in state 7), fn_710030bf8c's answer when the charge (+0x47c) and count
// (+0x454) are positive or the meter at +0x10 of +0xe8 of +0xc0 is full (+0x44 >= +0x40), else false.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGhostAuraMeter_b66a2 {
    u8 pad_0x0[0x40];
    f32 max;
    f32 value;
};

struct UnkGhostAuraMeterHolder_b66a2 {
    u8 pad_0x0[0x10];
    UnkGhostAuraMeter_b66a2* meter;
};

struct UnkGhostAuraOwner_b66a2 {
    u8 pad_0x0[0xe8];
    UnkGhostAuraMeterHolder_b66a2* meters;
};

struct UnkGhostAuraSubState_b66a2 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkGhostAura_b66a2 {
    u8 pad_0x0[0x20];
    UnkGhostAuraSubState_b66a2* sub;
    u8 pad_0x28[0x8c];
    s32 state;
    u8 pad_0xb8[0x8];
    UnkGhostAuraOwner_b66a2* owner;
    u8 pad_0xc8[0x38c];
    s32 count;
    u8 pad_0x458[0x24];
    f32 charge;
};

bool fn_710030bf8c(UnkGhostAura_b66a2* self);

bool fn_71003091a0(UnkGhostAura_b66a2* self) {
    s32 state = self->state;
    if (state == 7) {
        if (!self->sub) {
            return false;
        }
        state = self->sub->state;
    }
    if (state != 4) {
        return false;
    }
    if (!(self->charge > 0.0f && self->count > 0)) {
        UnkGhostAuraMeterHolder_b66a2* meters = self->owner->meters;
        if (!meters) {
            return false;
        }
        UnkGhostAuraMeter_b66a2* meter = meters->meter;
        if (!meter) {
            return false;
        }
        if (!(meter->value >= meter->max)) {
            return false;
        }
    }
    return fn_710030bf8c(self);
}

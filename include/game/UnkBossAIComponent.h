#pragma once

// The AI component of the boss character classes (the unit tools/tu_map.py names after
// CCharClassBossAIComponent). The concrete components derive from it and add their own members
// from +0x528. Only the members the sources share are spelled out; the name and the layout are
// placeholders.

#include "types.h"

struct UnkBossAIComponent {
    void** vtable;
    u8 unk_0x8[0x399];
    u8 field_0x3a1;
    u8 unk_0x3a2[0x67];
    u8 field_0x409;
    u8 unk_0x40a;
    u8 field_0x40b;
    u8 unk_0x40c[0xf0];
    u8 field_0x4fc;
    u8 field_0x4fd;
    u8 unk_0x4fe[0x2a];
};
static_assert(sizeof(UnkBossAIComponent) == 0x528, "UnkBossAIComponent placeholder layout");

// The constructor (the base constructor fn_7100416db8, the vtable, then its flags and defaults).
// Called first by the constructors of the boss components.
void fn_710041a198(UnkBossAIComponent* self);

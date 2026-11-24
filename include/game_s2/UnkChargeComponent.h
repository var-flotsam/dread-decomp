#pragma once

// A component with a charge and the tunable it reads its rates from (g_unk_71726e0aa8, built by
// src/game/CEmmyOverrideDeathPositionDef/fn_7100536550.cpp's unit initialiser); only what fn_7100502628 and fn_71005028e4 touch
// is named.

#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkChargeTunable : UnkTunable {
    u8 pad_0x8[0x94];
    f32 field_0x9c;
    f32 field_0xa0;
    f32 field_0xa4;
    f32 field_0xa8;
    f32 field_0xac;
    u8 pad_0xb0[0x28];
    f32 field_0xd8[2];
    u8 pad_0xe0[0xc];
    f32 field_0xec;
    u8 pad_0xf0[0xc];
    f32 field_0xfc;  // the shortest time a target is kept (CTunableAIManager_SetTarget_ldaec80.cpp)
};

struct UnkFlagged {
    u8 pad_0x0[0x64];
    // A union member: no type-based alias info, so the owner pointer is reloaded after the store.
    union {
        u8 field_0x64;
    };
};

struct UnkWorld;

struct UnkChargeOwner {
    u8 pad_0x0[0x1b8];
    UnkFlagged* field_0x1b8;
    u8 pad_0x1c0[0xc0];
    UnkWorld* field_0x280;  // the FX world (fn_71005026d0)
    u8 pad_0x288[0x18];
    void* field_0x2a0;
};

struct UnkScale {
    u8 pad_0x0[0xd4];
    f32 field_0xd4;
};

struct UnkChargeComponent {
    u8 pad_0x0[0x30];
    UnkChargeOwner* field_0x30;
    u8 pad_0x38[0x44];
    f32 field_0x7c;
    u8 pad_0x80[0x4f9];
    u8 field_0x579;
    u8 pad_0x57a[0x56];
    UnkScale* field_0x5d0;
    u8 pad_0x5d8[4];
    f32 field_0x5dc;
    f32 field_0x5e0;
    u8 field_0x5e4;
    u8 pad_0x5e5[0x13];
    f32 field_0x5f8;
};

extern UnkChargeTunable g_unk_71726e0aa8;

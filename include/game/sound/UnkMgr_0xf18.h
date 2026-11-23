#pragma once

// The 0xf18-byte manager singleton g_unk_71726bc560 (include/game/CBossAIComponent.h), as
// src/game_7100ece7fc/fn_7100fce86c.cpp reads it: a state override at 0x38c and the object at 0x2f8 whose
// +0x10 is the state. Names and layouts are placeholders.
#include "types.h"

struct UnkState_fce86c {
    u8 unk_0x0[0x10];
    s32 state;
};

struct UnkMgr_0xf18 {
    u8 unk_0x0[0x2f8];
    UnkState_fce86c* field_0x2f8;
    u8 unk_0x300[0x8c];
    s32 field_0x38c;
    u8 unk_0x390[0xb88];
};
static_assert(sizeof(UnkMgr_0xf18) == 0xf18, "layout");

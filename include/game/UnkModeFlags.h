#pragma once

#include "types.h"

// The mode globals (g_unk_71726bc570, game/CBossAIComponent.h). The flag at +0xb8, as
// src/game/GUI/CIngameMenu/fn_7100b2de48.cpp sees it: set, the step callbacks and the blackboard pass
// are skipped. The word at +0x100 is what UnkTU1734_Button_ovb0202.cpp refreshes.
struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
    u8 unk_0xb9[0x47];
    u32 field_0x100;
};
static_assert(__builtin_offsetof(UnkModeFlags, field_0x100) == 0x100, "UnkModeFlags placeholder layout");

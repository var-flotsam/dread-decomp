#pragma once

#include "types.h"

// A flag holder (as src/game/GUI/CIngameMenu/fn_7100b2de48.cpp sees it): set, the step callbacks and the
// blackboard pass are skipped.
struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

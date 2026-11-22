#pragma once

#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU2527.h"

// As src/game_s7/fn_7100d92158.cpp defines it.
// A registry entry (built out of line by fn_710002003c).
struct UnkEntry_7100d92158 {
    UnkDesc_7100d92158* desc;
    void* factory;
    UnkStrIdTwoStep id;
    UnkPathIdTwoStep path_0x18;
    UnkPathIdTwoStep path_0x20;
    u64 field_0x28;
    void* field_0x30;
    u64 field_0x38;
    void* field_0x40;

    UnkEntry_7100d92158(UnkDesc_7100d92158* desc, void* factory);  // fn_710002003c
};

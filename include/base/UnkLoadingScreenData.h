#pragma once

#include "game/UnkTUInit.h"

// The TU's data: the header, then a pair of floats (-1, -1) and a flag.
struct UnkLoadingScreenData {
    UnkTUHeader header;
    u8 unk_0x18[0x80];
    f32 field_0x98;
    f32 field_0x9c;
    bool field_0xa0;
};

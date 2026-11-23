#pragma once

#include "game/UnkTUInit.h"

// The header built the other way round: "sources" first, the word last.
struct UnkTUHeaderRev {
    u32 field_0x0;
    CStrId sources;
    CStrId binaries;
};

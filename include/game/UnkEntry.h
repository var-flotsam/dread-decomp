#pragma once

#include "game/UnkKind.h"
#include "types.h"

struct UnkEntry {
    u8 pad_0x0[0x10];
    UnkKind* field_0x10;
};

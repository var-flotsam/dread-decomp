#pragma once

#include "types.h"

struct UnkMissionLog {
    u8 pad_0x0[0x248];
    u8 entries_0x248[0x28];
    u8 entries_0x270[0x28];
};

#pragma once

#include "game/UnkPolyBase.h"

// Each level of the class chain copies its own members, so the runs are not merged.
struct UnkLevel0 : UnkPolyBase {
    u32 field_0x8[8];
};

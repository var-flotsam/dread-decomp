#pragma once

#include "base/UnkPairArrays.h"

struct UnkPairData {
    u8 pad_0x0[0xf0];
    base::UnkPairArrays arrays;
};

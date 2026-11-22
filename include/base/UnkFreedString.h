#pragma once

#include "types.h"

struct alignas(16) UnkFreedString {
    u64 unk_0x0[6];

    ~UnkFreedString();
};

#pragma once

#include "types.h"

// The functions of a held type (only the destroy slot is named).
struct UnkHeldType_f29a0 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* storage);
};

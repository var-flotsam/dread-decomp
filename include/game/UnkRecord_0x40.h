#pragma once

#include "types.h"

// And over 0x40-byte records held at 8 in an object, with the key passed by value
// (fn_7100015250, fn_7100015508).
struct UnkRecord_0x40 {
    u8 data[0x40];
};

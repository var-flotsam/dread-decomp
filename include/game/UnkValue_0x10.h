#pragma once

#include "types.h"

// The lookup and the removal over 0x10-byte values (fn_71009046bc, fn_7100904644) and 0x80-byte values
// (fn_7100e6095c, fn_7100e608e0), each removal with its own out-of-line removal.
struct UnkValue_0x10 {
    u64 words[2];
};

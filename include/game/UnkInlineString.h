#pragma once

#include "types.h"

// A string with 0x80 bytes of inline storage (the string part is 0x30 bytes, followed by
// its inline allocator).
// A string with 0x80 bytes of inline storage (as in fn_7100ae9a10).
struct UnkInlineString {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};

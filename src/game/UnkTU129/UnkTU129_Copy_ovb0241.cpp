// Unit 129 of tools/tu_map.py (no reliable name): copies the 23 words at +0x20 of a secondary base
// (the primary object's block at +0x40) to an output block unless it is that block, and returns true.
// Names and layouts are placeholders.
#include "types.h"

struct UnkWords_a4a2c {
    u32 words[23];
};

// The object whose secondary base the method is called on (0x20 bytes in).
struct UnkPrimary_a4a2c {
    u8 pad_0x0[0x40];
    UnkWords_a4a2c value;  // 0x40
};

bool fn_7100144a2c(void* self, UnkWords_a4a2c* out) {
    UnkPrimary_a4a2c* primary = reinterpret_cast<UnkPrimary_a4a2c*>(static_cast<char*>(self) - 0x20);
    if (&primary->value != out) {
        // keeps the primary base in its own register, as the original addresses the fields from it
        asm("" : "+r"(primary));
        for (u32 i = 0; i < 23; ++i)
            out->words[i] = primary->value.words[i];
    }
    return true;
}

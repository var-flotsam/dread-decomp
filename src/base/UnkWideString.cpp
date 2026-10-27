// A conversion from the game's 32-bit character strings to base::global::CRntString (from the unit
// tools/tu_map.py names after nn::time, unit 45). Holds fn_710006c080.
#include <string.h>

#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// Narrows a zero-terminated string of 32-bit characters (each truncated to a byte, at most 4095 of
// them) and returns it as a string.
CRntString fn_710006c080(const u32* text) {
    char buffer[0x1000];
    memset(buffer, 0, sizeof(buffer));
    u32 i = 0;
    for (u32 c = text[0]; c != 0; c = text[i])
        buffer[i++] = (char)c;
    buffer[i] = 0;
    return CRntString(buffer);
}

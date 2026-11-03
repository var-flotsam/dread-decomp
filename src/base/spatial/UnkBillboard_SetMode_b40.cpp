// From unit 243 of tools/tu_map.py (the billboard of src/base/spatial/UnkBillboard.cpp): sets the
// blend mode (kept at 0x2f0) unless the billboard is locked (the byte at 0x375), writing the matching
// words into its material at 0x1a8. The word at +0x18 has no type-based alias information (the
// original reloads the material after storing it). Names and layouts are placeholders; fields are
// written by offset.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32;

void fn_71001b7584(char* self, u32 mode) {
    if (*(bool*)(self + 0x375))
        return;
    *(u32*)(self + 0x2f0) = mode;
    u32 src, dst;
    if (mode == 0) {
        *(UnkAliasU32*)(*(char**)(self + 0x1a8) + 0x18) = 2;
        src = 6;
        dst = 7;
    } else if (mode == 1) {
        *(UnkAliasU32*)(*(char**)(self + 0x1a8) + 0x18) = 8;
        src = 6;
        dst = 1;
    } else {
        src = 1;
        dst = 0;
    }
    char* m = *(char**)(self + 0x1a8);
    *(u32*)(m + 0x34) = 0;
    *(u32*)(m + 0x38) = src;
    *(u32*)(m + 0x3c) = dst;
}

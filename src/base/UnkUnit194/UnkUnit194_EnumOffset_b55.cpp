// Unit 194 (tools/tu_map.py): maps the values 0-2 of a small enum onto 0x5c-0x5e, anything else
// to -1.
#include "types.h"

s32 fn_7100167ce0(u32 value) {
    switch (value) {
    case 0: return 0x5c;
    case 1: return 0x5d;
    case 2: return 0x5e;
    default: return -1;
    }
}

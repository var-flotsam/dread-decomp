// Unit 331 (tools/tu_map.py): two small enum conversions, value + 1 for 0..N and 0 beyond.
#include "types.h"

s32 fn_7100225550(u32 value) {
    switch (value) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    case 4: return 5;
    default: return 0;
    }
}

s32 fn_7100225570(u32 value) {
    switch (value) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    default: return 0;
    }
}

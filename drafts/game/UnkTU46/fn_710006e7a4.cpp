#include "types.h"

struct UnkPadButtons_b0232;

s32 fn_710006e7a4(UnkPadButtons_b0232* self, s32 button) {
    s32 r = 0;
    switch (button) {
        case 0x2: r = 1; break;
        case 0x4: r = 0xb; break;
        case 0x8: r = 0xa; break;
        case 0x10: r = 0xe; break;
        case 0x20: r = 0xc; break;
        case 0x40: r = 0xd; break;
        case 0x80: r = 0xf; break;
        case 0x100: r = 7; break;
        case 0x200: r = 6; break;
        case 0x400: r = 2; break;
        case 0x800: r = 3; break;
        case 0x1000: r = 5; break;
        case 0x2000: r = 4; break;
        case 0x4000: r = 0x15; break;
        case 0x8000: r = 0x17; break;
        case 0x10000: r = 0x15; break;
        case 0x20000: r = 0x17; break;
        case 0x40000: r = 2; break;
        case 0x100000: r = 9; break;
        case 0x200000: r = 8; break;
        default: break;
    }
    return r;
}

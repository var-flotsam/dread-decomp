// Byte size for a packed (kind | count << 16) descriptor: kind 1 is 2 bytes per
// element, kinds 2 and 3 are 4 bytes per element, anything else is 0.
#include "types.h"

u32 fn_7100162300(u32 desc) {
    u32 count = desc >> 16;
    switch (desc & 0xffff) {
    case 1:
        return count * 2;
    case 2:
    case 3:
        return count * 4;
    default:
        return 0;
    }
}

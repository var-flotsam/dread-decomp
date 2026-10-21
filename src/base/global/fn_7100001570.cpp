// The 64-bit string hash behind CStrId and CName: a table-driven, reflected CRC-64 with an
// all-ones initial value and no final xor, unrolled by 8 and then by 4.
#include "types.h"

extern const u64 g_unk_7101646d68[256];  // the CRC table (reached through the GOT)

#define CRC64_STEP(i) crc = g_unk_7101646d68[(crc ^ (u8)str[i]) & 0xff] ^ (crc >> 8)

u64 fn_7100001570(const char* str, u64 length) {
    u64 crc = ~0ul;
    while (length > 8) {
        CRC64_STEP(0);
        CRC64_STEP(1);
        CRC64_STEP(2);
        CRC64_STEP(3);
        CRC64_STEP(4);
        CRC64_STEP(5);
        CRC64_STEP(6);
        CRC64_STEP(7);
        str += 8;
        length -= 8;
    }
    while (length > 4) {
        CRC64_STEP(0);
        CRC64_STEP(1);
        CRC64_STEP(2);
        CRC64_STEP(3);
        str += 4;
        length -= 4;
    }
    while (length) {
        CRC64_STEP(0);
        ++str;
        --length;
    }
    return crc;
}

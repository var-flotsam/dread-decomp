// Unit 55 (tools/tu_map.py, no reliable name): the table-driven CRC-32 update over a byte range,
// unrolled eight, then four, then one byte at a time. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "types.h"

extern u32 g_unk_7101a1a6c0[256];

#define CRC_STEP(crc, byte) ((crc) = g_unk_7101a1a6c0[((crc) ^ (byte)) & 0xff] ^ ((crc) >> 8))

// CRC-32 update over `length` bytes, eight, then four, then one byte at a time.
u32 fn_710007b750(void* dest, u64 buffer, s32 count) {
    const u8* data = static_cast<const u8*>(dest);
    u64 length = buffer;
    u32 crc = count;
    while (length > 8) {
        CRC_STEP(crc, data[0]);
        CRC_STEP(crc, data[1]);
        CRC_STEP(crc, data[2]);
        CRC_STEP(crc, data[3]);
        CRC_STEP(crc, data[4]);
        CRC_STEP(crc, data[5]);
        CRC_STEP(crc, data[6]);
        CRC_STEP(crc, data[7]);
        data += 8;
        length -= 8;
    }
    while (length > 4) {
        CRC_STEP(crc, data[0]);
        CRC_STEP(crc, data[1]);
        CRC_STEP(crc, data[2]);
        CRC_STEP(crc, data[3]);
        data += 4;
        length -= 4;
    }
    while (length) {
        CRC_STEP(crc, *data);
        data++;
        length--;
    }
    return crc;
}

// The 64-bit string hash behind CStrId and CName: a table-driven, reflected CRC-64 with an
// all-ones initial value and no final xor, unrolled by 8 and then by 4.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkStartPoint.h"
#include "types.h"

extern const u64 g_unk_7101646d68[/* 256; unsized: x86-64 would assume 16-byte alignment */];  // the CRC table (reached through the GOT)

#define CRC64_STEP(i) crc = g_unk_7101646d68[(crc ^ (u8)str[i]) & 0xff] ^ (crc >> 8)

u64 fn_7100001570(const char* str, u64 length) {
    u64 crc = ~0ull;
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

// The unit's static initialiser (fn_7100001760): the header (6, the "binaries" and "sources" ids),
// two limits of -1 and a flag, and the start record (game/UnkStartPoint.h). The compiler merged the
// unit's statics into one block from 0x7101d4738c, which is laid out here as it placed them.
struct UnkUnit2Statics {
    u32 header;                 // 6
    u8 pad_0x4[0x10];
    u8 binaries[8];             // +0x14: CStrId
    u8 sources[8];              // +0x1c: CStrId
    u8 limits[8];               // +0x24: two f32, -1
    u8 field_0x2c;
    u8 pad_0x2d[7];
    u8 start[sizeof(UnkStartPoint)];  // +0x34: 0x7101d473c0
};
extern UnkUnit2Statics g_unk_7101d4738c __attribute__((visibility("hidden")));

void fn_710007e6c4(void* self);  // an empty CRntString

// NON_MATCHING(minor): register allocation only: the original keeps the "binaries" id's address in
// a register and reaches "sources" through the header store's post-increment, this the other way
// round; the same stores and calls with the same arguments, in the same order.
void fn_7100001760() {
    using base::global::CRntString;
    using base::global::CStrId;
    UnkUnit2Statics& s = g_unk_7101d4738c;
    s.header = 6;
    new (s.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, s.binaries, g_unk_7101a19000);
    new (s.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, s.sources, g_unk_7101a19000);
    UnkStartPoint* start = reinterpret_cast<UnkStartPoint*>(s.start);
    start->field_0x0 = 0;
    *reinterpret_cast<u64*>(s.limits) = 0xbf800000bf800000;
    s.field_0x2c = 1;
    start->field_0x4 = true;
    fn_710007e6c4(&start->field_0x8);
    fn_710007e6c4(&start->field_0x38);
    new (&start->character) CRntString("samus");
    new (&start->start_point) CRntString("StartPoint0");
    fn_710007e6c4(&start->field_0xc8);
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100000584), start, g_unk_7101a19000);
}

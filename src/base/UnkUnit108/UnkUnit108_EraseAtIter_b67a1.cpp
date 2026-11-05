// tools/tu_map.py unit 108: reflected collection slot 0x128, erases the 0x48-byte entry an index
// iterator names through fn_710011f95c (fn_710011ed4c). The index goes through a 64-bit byte offset
// from the collection itself and a pointer difference, as
// src/game/CThermalDeviceComponent/UnkEraseAtIter_A6_b67.cpp found. Names are placeholders.
#include "types.h"

struct UnkEntryVector108_b67a1;

struct UnkEntry48_b67a1 {
    u8 unk_0x0[0x48];
};

void fn_710011f95c(UnkEntryVector108_b67a1* v, u32 index);

void fn_710011ed4c(UnkEntryVector108_b67a1* self, const u32* it) {
    UnkEntry48_b67a1* base = reinterpret_cast<UnkEntry48_b67a1*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry48_b67a1);
    UnkEntry48_b67a1* p = reinterpret_cast<UnkEntry48_b67a1*>(reinterpret_cast<char*>(base) + offset);
    fn_710011f95c(self, (u32)(p - base));
}

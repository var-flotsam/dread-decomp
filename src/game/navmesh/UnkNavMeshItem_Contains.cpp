// From unit 372 of tools/tu_map.py (the navigation-mesh item of UnkNavMeshItem.cpp): whether the
// item's list of 0x40-byte records at +0xf0 (count at +0xf8) holds one whose first word is the
// given key. Names and layouts are placeholders.
#include "types.h"

struct UnkNavMeshItemRecord {
    u64 key;
    u8 pad_0x8[0x38];
};

struct UnkNavMeshItemRecords {
    u8 pad_0x0[0xf0];
    UnkNavMeshItemRecord* records;  // 0xf0
    u32 count;                      // 0xf8
};

bool fn_71002b3c60(UnkNavMeshItemRecords* self, const u64* key) {
    for (UnkNavMeshItemRecord *it = self->records, *end = it + self->count; it != end; ++it) {
        if (it->key == *key)
            return true;
    }
    return false;
}

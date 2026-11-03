// CWorldGraph unit (tools/tu_map.py): the index of the 0x80-byte entry (array at +0x30, count at +0x38)
// whose key at +8 equals the given one, or -1. Names and layouts are placeholders.
#include "types.h"

struct UnkWorldGraphEntry_b47a3 {
    u8 unk_0x0[8];
    u64 key;
    u8 unk_0x10[0x70];
};

struct UnkWorldGraphEntries_b47a3 {
    u8 unk_0x0[0x30];
    UnkWorldGraphEntry_b47a3* entries;
    u32 count;
};

s32 fn_7100acca04(UnkWorldGraphEntries_b47a3* self, const u64* key) {
    for (u64 i = 0; i < self->count; ++i) {
        if (self->entries[i].key == *key)
            return (s32)i;
    }
    return -1;
}

// CSubAreaManager unit (tools/tu_map.py): looks up the 0x18-byte entry with both keys in the list at +0x990
// (count +0x998) and copies its two floats out where they are positive (the second to `second`, the first to
// `first`); false when there is no such entry. Names and layouts are placeholders.
#include "types.h"

struct UnkPairEntry_A6_b72 {
    u64 key0;
    u64 key1;
    f32 first;
    f32 second;
};

struct UnkPairTable_A6_b72 {
    u8 pad_0x0[0x990];
    UnkPairEntry_A6_b72* entries;
    u32 count;
};

bool fn_7100eeb670(UnkPairTable_A6_b72* self, const u64* key0, const u64* key1, f32* second, f32* first) {
    u64 k0 = *key0;
    u64 k1 = *key1;
    for (UnkPairEntry_A6_b72 *e = self->entries, *end = e + self->count; e != end; ++e) {
        if (e->key0 == k0 && e->key1 == k1) {
            if (e->first > 0.0f)
                *first = e->first;
            if (e->second > 0.0f)
                *second = e->second;
            return true;
        }
    }
    return false;
}

// CHeatableShieldComponent's unit: for the entry keyed by `key` in the flat map at +0x40 (0xd0-byte
// values, u64 keys at +0x48, count at +0x50), sets the flag at +0xa2 and clears +0x60. Names are placeholders.
#include "types.h"

struct UnkHeatableShieldEntry_A5_b66 {
    u8 pad_0x0[0x60];
    u32 field_0x60;
    u8 pad_0x64[0xa2 - 0x64];
    bool field_0xa2;
    u8 pad_0xa3[0xd0 - 0xa3];
};

struct UnkHeatableShieldMap_A5_b66 {
    u8 pad_0x0[0x40];
    UnkHeatableShieldEntry_A5_b66* values;
    u64* keys;
    u32 count;

    UnkHeatableShieldEntry_A5_b66* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

void fn_710086c7a8(UnkHeatableShieldMap_A5_b66* self, const u64* key, bool value) {
    UnkHeatableShieldEntry_A5_b66* it = self->Find(*key);
    if (it != self->values + self->count) {
        it->field_0xa2 = value;
        it->field_0x60 = 0;
    }
}

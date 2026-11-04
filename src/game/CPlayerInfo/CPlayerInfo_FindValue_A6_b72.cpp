// CPlayerInfo unit (tools/tu_map.py): a flat map (values of 0x30 bytes at +0x320, keys at +0x328, count at
// +0x330) looked up by key; the float at +4 of the found value, or 0. Names and layouts are placeholders.
#include "types.h"

struct UnkInfoValue_A6_b72 {
    u32 field_0x0;
    f32 amount;
    u8 pad_0x8[0x28];
};

struct UnkInfoMap_A6_b72 {
    u8 pad_0x0[0x320];
    UnkInfoValue_A6_b72* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkInfoValue_A6_b72* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

f32 fn_7100f0ec68(UnkInfoMap_A6_b72* self, const u64* key) {
    UnkInfoValue_A6_b72* v = self->Find(*key);
    if (v == self->values + self->count)
        return 0.0f;
    return v->amount;
}

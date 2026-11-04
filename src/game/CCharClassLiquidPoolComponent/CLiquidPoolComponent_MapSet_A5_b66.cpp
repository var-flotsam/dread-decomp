// CCharClassLiquidPoolComponent's unit: copies a 16-byte record into the entry keyed by `key` in the
// flat map (values at +0, u64 keys at +8, count at +0x10), when the key is there. Names are placeholders.
#include "types.h"

struct UnkLiquidPoolRecord_A5_b66 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkLiquidPoolMap_A5_b66 {
    UnkLiquidPoolRecord_A5_b66* values;
    u64* keys;
    u32 count;

    UnkLiquidPoolRecord_A5_b66* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

void fn_7100904728(UnkLiquidPoolMap_A5_b66* self, const u64* key, const UnkLiquidPoolRecord_A5_b66* value) {
    UnkLiquidPoolRecord_A5_b66* it = self->Find(*key);
    if (it != self->values + self->count) {
        it->field_0x0 = value->field_0x0;
        it->field_0x4 = value->field_0x4;
        it->field_0x8 = value->field_0x8;
        it->field_0xc = value->field_0xc;
    }
}

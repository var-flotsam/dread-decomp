// CCharClassAttackComponent (unit 1185): looks a string id up in the component's table of keys and
// returns the matching value, or the empty string id when there is none. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkAttackTable_l78d46c {
    u8 pad_0x0[0xa0];
    base::global::CStrId* values;  // 0xa0
    u64* keys;                     // 0xa8
    u32 count;                     // 0xb0

    base::global::CStrId* End() { return &values[count]; }
    base::global::CStrId* Find(const u64* key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return &values[i];
        return End();
    }
    bool Has(const u64* key) { return Find(key) != End(); }
    base::global::CStrId* At(const u64* key) {
        base::global::CStrId* value = values;
        const u64* k = keys;
        for (u64 n = count; n; ++value, --n, ++k)
            if (*k == *key)
                return value;
        return End();
    }
};

base::global::CStrId* fn_710078d46c(UnkAttackTable_l78d46c* self, const u64* key) {
    if (!self->Has(key))
        return &g_unk_7101d470e0;
    return self->At(key);
}

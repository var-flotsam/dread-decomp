// Unit 2200: copies a value into the entry stored under a key in a keyed array (keys in one array, 0x18-byte values in
// another): its two ids and word are copy-assigned. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkValue_lc90760 {
    CStrId field_0x0;
    CStrId field_0x8;
    u32 field_0x10;
};

struct UnkKeyedValues_lc90760 {
    UnkValue_lc90760* values;
    u64* keys;
    u32 count;

    UnkValue_lc90760* Find(const u64* key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == *key)
                return &values[i];
        }
        return &values[count];
    }
};

void fn_7100c90760(UnkKeyedValues_lc90760* self, const u64* key, const UnkValue_lc90760* value) {
    UnkValue_lc90760* it = self->Find(key);
    if (it != &self->values[self->count]) {
        it->field_0x0 = value->field_0x0;
        it->field_0x8 = value->field_0x8;
        it->field_0x10 = value->field_0x10;
    }
}

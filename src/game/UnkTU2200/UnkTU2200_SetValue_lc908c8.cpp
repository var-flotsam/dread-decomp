// Unit 2200: sets the value stored under a key in a keyed array (keys in one array, 0x18-byte values in
// another): the value's two ids are moved in, its word copied. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkValue_lc908c8 {
    CStrId field_0x0;
    CStrId field_0x8;
    u32 field_0x10;
};

struct UnkKeyedValues_lc908c8 {
    UnkValue_lc908c8* values;
    u64* keys;
    u32 count;

    UnkValue_lc908c8* Find(const u64* key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == *key)
                return &values[i];
        }
        return &values[count];
    }
};

void fn_7100c908c8(UnkKeyedValues_lc908c8* self, const u64* key, UnkValue_lc908c8* value) {
    UnkValue_lc908c8* it = self->Find(key);
    if (it != &self->values[self->count]) {
        it->field_0x0.ReleaseAndTake(value->field_0x0);
        it->field_0x8.ReleaseAndTake(value->field_0x8);
        it->field_0x10 = value->field_0x10;
    }
}

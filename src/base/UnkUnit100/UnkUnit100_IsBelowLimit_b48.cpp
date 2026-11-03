// Unit 100 (tools/tu_map.py): for the entry of `key` in a table of 12-byte entries (data +0x8,
// count +0x10), whether its value is below its limit; without an entry, whether the default flag
// (+0x0) is set. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit100Entry {
    u32 key;
    u32 limit;
    u32 value;
};

struct UnkUnit100Table {
    u32 default_flag;
    UnkUnit100Entry* data;  // 0x8
    u32 count;              // 0x10
};

static inline UnkUnit100Entry* Find(UnkUnit100Table* self, const u32* pkey) {
    UnkUnit100Entry* e = self->data;
    u32 key = *pkey;
    u64 n = self->count;
    do {
        if (e->key == key)
            return e;
        ++e;
    } while (--n != 0);
    return nullptr;
}

bool fn_7100104a94(UnkUnit100Table* self, const u32* key) {
    if (self->count != 0) {
        if (UnkUnit100Entry* e = Find(self, key))
            return e->value < e->limit;
    }
    return self->default_flag != 0;
}

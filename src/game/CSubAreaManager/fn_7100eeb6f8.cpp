// From CSubAreaManager's unit: the 0x188-byte record (in the array at +0x860, count at +0x868) whose
// first word is the given key, or null. Names and layouts are placeholders.
#include "types.h"

struct UnkSubAreaRecord {
    u64 key;
    u8 pad_0x8[0x180];
};

struct UnkSubAreaRecords {
    u8 pad_0x0[0x860];
    UnkSubAreaRecord* data;
    u32 count;
};

UnkSubAreaRecord* fn_7100eeb6f8(UnkSubAreaRecords* self, const u64* key) {
    for (UnkSubAreaRecord *it = self->data, *end = self->data + self->count; it != end; ++it)
        if (it->key == *key)
            return it;
    return nullptr;
}

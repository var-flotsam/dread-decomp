// Unit 23: appends a (key, 0x40-byte record) pair to a keyed array, growing it by its step when full,
// and returns the last record. Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "types.h"

// Words that may alias anything: the array's fields are reread after the copies.
typedef u32 __attribute__((may_alias)) UnkAliasWord;

struct UnkWordRecord_0x40 {
    UnkAliasWord words[16];
};

using UnkRecordsByKeyStore = base::global::CRntSmallDictionary<u64, UnkWordRecord_0x40>;

void fn_7100015378(UnkRecordsByKeyStore* self, u32 capacity);

UnkWordRecord_0x40& fn_7100015b6c(UnkRecordsByKeyStore* self, const u64* key, const UnkWordRecord_0x40* record) {
    if (self->count == self->capacity)
        fn_7100015378(self, self->count + self->step);
    __builtin_memcpy(&self->keys[self->count], key, sizeof(u64));
    UnkWordRecord_0x40* dst = &self->values[self->count];
    for (u32 w = 0; w < 16; ++w)
        dst->words[w] = record->words[w];
    u32 n = ++self->count;
    UnkWordRecord_0x40* end = &self->values[n];
    return n == 0 ? *end : end[-1];
}

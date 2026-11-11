// Unit 2200: copy constructor of a table of refcounted entries (pairs of string-data pointers
// plus a second array of single pointers). Names and layouts are placeholders.

#include "base/global/CStrId.h"

using base::global::UnkStringData;

struct UnkRefPair {
    UnkStringData* first;
    UnkStringData* second;
    u32 field_0x10;
    u32 pad_0x14;
};

struct UnkRefTable {
    UnkRefPair* pairs;
    UnkStringData** singles;
    u32 count;
    u32 pad_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
};

void fn_7100c8bec8(UnkRefTable* self, u32 count);

static inline void AddRef(UnkStringData* d) {
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

void fn_7100c8bd98(UnkRefTable* self, const UnkRefTable* other) {
    self->singles = nullptr;
    self->pairs = nullptr;
    __builtin_memset(&self->singles, 0, 16);
    self->pairs = nullptr;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->field_0x20 = other->field_0x20;
    fn_7100c8bec8(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        const UnkRefPair* src = &other->pairs[i];
        UnkRefPair* dst = &self->pairs[i];
        __builtin_memcpy(&dst->first, &src->first, 8);
        AddRef(src->first);
        __builtin_memcpy(&dst->second, &src->second, 8);
        AddRef(src->second);
        __builtin_memcpy(&dst->field_0x10, &src->field_0x10, 4);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* d = other->singles[i];
        __builtin_memcpy(&self->singles[i], &d, 8);
        AddRef(d);
    }
}

// Unit 134 of tools/tu_map.py: finds the index of the entry whose object (entry->+0x10->+0x30) is `key`
// in the list reached through +0x160 then +0x78 (entries at +0x40, count at +0x24); the index (or -1)
// goes to `out` (fn_7100146d0c and its identical copy fn_71001472b0). Names and layouts are placeholders.
#include "types.h"

struct UnkEntryInner134 {
    u8 pad_0x0[0x30];
    void* key;  // 0x30
};

struct UnkEntry134 {
    u8 pad_0x0[0x10];
    UnkEntryInner134* inner;  // 0x10
};

struct UnkEntryList134 {
    u8 pad_0x0[0x24];
    u32 count;  // 0x24
    u8 pad_0x28[0x18];
    UnkEntry134** entries;  // 0x40
};

struct UnkHolder134 {
    u8 pad_0x0[0x78];
    UnkEntryList134* list;  // 0x78
};

struct UnkOwner134 {
    u8 pad_0x0[0x160];
    UnkHolder134* holder;  // 0x160
};

// The index is written through a type that may alias the list's pointers (the original reloads them).
typedef s32 __attribute__((may_alias)) UnkAliasS32;

__attribute__((always_inline)) static inline bool FindIndex(UnkOwner134* self, void* key, UnkAliasS32* out) {
    *out = -1;
    u32 count = self->holder->list->count;
    if (count == 0)
        return false;
    u64 i = 0;
    bool found = false;
    s32 index = -1;
    do {
        if (self->holder->list->entries[i]->inner->key == key) {
            found = true;
            index = i;
            *out = i;
        }
    } while (++i < count && index == -1);
    return found;
}

bool fn_7100146d0c(UnkOwner134* self, void* key, UnkAliasS32* out) {
    return FindIndex(self, key, out);
}

// An identical copy.
bool fn_71001472b0(UnkOwner134* self, void* key, UnkAliasS32* out) {
    return FindIndex(self, key, out);
}

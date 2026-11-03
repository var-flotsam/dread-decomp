// A function in an unnamed unit (tu_map unit 1115, 0x71006f8120-0x71006fbadc): calls fn_71006f86c4 on
// every entry of the pointer array at +0x68 (count at +0x70) with the same argument. Names and layouts
// are placeholders.
#include "types.h"

struct UnkUnit1115Entry;
void fn_71006f86c4(UnkUnit1115Entry* entry, void* arg);

struct UnkUnit1115List {
    u8 unk_0x0[0x68];
    UnkUnit1115Entry** entries;
    u32 count;
};

void fn_71006fa84c(UnkUnit1115List* self, void* arg) {
    for (UnkUnit1115Entry **it = self->entries, **end = it + self->count; it != end; ++it)
        fn_71006f86c4(*it, arg);
}

// The value for a key: the index of the key in the u32 array at +0x48 (count at +0x50; the count when
// it is missing) picks the entry of the pointer array at +0x40.
struct UnkUnit1115Map {
    u8 unk_0x0[0x40];
    void** values;
    u32* keys;
    u32 count;
};

static inline u64 IndexOf(const UnkUnit1115Map* self, u32 key) {
    for (u64 i = 0; i < self->count; ++i) {
        if (self->keys[i] == key)
            return i;
    }
    return self->count;
}

void* fn_71006f9320(UnkUnit1115Map* self, u32 key) {
    u64 index = IndexOf(self, key);
    return self->values[index];
}

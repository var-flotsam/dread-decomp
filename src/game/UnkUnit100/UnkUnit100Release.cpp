// From unit 100 of tools/tu_map.py (0x71001043e4-0x7100104ef4, no reflected class): drops one use of
// a key in an object's table of counted keys (12-byte entries at +0x8, as many as the count at
// +0x10): the first entry with the key has its count decremented unless it is already zero. Names
// and layouts are placeholders.
#include "types.h"

struct UnkKeyUse {
    u32 key;
    u32 field_0x4;
    u32 uses;  // 0x8
};

struct UnkKeyUseTable {
    void* vtable;
    UnkKeyUse* entries;  // 0x8
    u32 count;           // 0x10

    UnkKeyUse* Find(u32 key) {
        for (u64 i = 0; i < count; i++)
            if (entries[i].key == key)
                return &entries[i];
        return nullptr;
    }
};

void fn_7100104ae4(UnkKeyUseTable* self, const u32* key) {
    UnkKeyUse* use = self->Find(*key);
    if (use && use->uses != 0)
        --use->uses;
}

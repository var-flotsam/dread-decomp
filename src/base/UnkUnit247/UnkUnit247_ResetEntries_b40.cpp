// Units 247 and 253 of tools/tu_map.py (no reflected class): the same reset in each, which clears the
// word at +0x10 of every present entry in the array of the object at 0x78. The word has no type-based
// alias information (the original reloads the array after each store). Names and layouts are
// placeholders.
#include "game/UnkTU247.h"
#include "game/UnkTU253.h"
#include "types.h"

struct UnkResetEntry {
    u8 pad_0x0[0x10];
    union {
        u32 field_0x10;
    };
};

struct UnkResetTable {
    u8 pad_0x0[0x30];
    u32 count;
    u8 pad_0x34[0x1c];
    UnkResetEntry** entries;  // 0x50
};

struct UnkResetOwner {
    u8 pad_0x0[0x78];
    UnkResetTable* table;
};

static inline __attribute__((always_inline)) void ResetEntries(UnkResetOwner* self) {
    u32 count = self->table->count;
    for (u64 i = 0; i < count; ++i) {
        if (UnkResetEntry* entry = self->table->entries[i])
            entry->field_0x10 = 0;
    }
}

void fn_71001bc068(UnkResetOwner* self) { ResetEntries(self); }
void fn_71001c3270(UnkResetOwner* self) { ResetEntries(self); }

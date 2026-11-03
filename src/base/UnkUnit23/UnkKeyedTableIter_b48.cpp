// Unit 23 (tools/tu_map.py): steps a cursor over a keyed table of 0x40-byte entries (entries +0x8,
// keys +0x10, count +0x18, cursor +0x30), returning the next key and entry. Names and layouts are
// placeholders.
#include "types.h"

struct UnkKeyedEntry {
    u8 data[0x40];
};

struct UnkKeyedTable {
    void* vtable;
    UnkKeyedEntry* entries;  // 0x8
    void** keys;             // 0x10
    u32 count;               // 0x18
    u8 pad_0x1c[0x14];
    UnkKeyedEntry* cursor;   // 0x30
};

// The keys are pointer-sized handles (-1 when none): the original reloads the cursor after the
// store through the key.
bool fn_7100015570(UnkKeyedTable* self, void** key, UnkKeyedEntry** entry) {
    *key = reinterpret_cast<void*>(~0ull);
    *entry = nullptr;
    UnkKeyedEntry* entries = self->entries;
    UnkKeyedEntry* end = entries + self->count;
    UnkKeyedEntry* cursor = self->cursor;
    if (cursor == end)
        return false;
    u32 index = (u32)(cursor - entries);
    *key = self->keys[index];
    *entry = self->cursor;
    ++self->cursor;
    return true;
}

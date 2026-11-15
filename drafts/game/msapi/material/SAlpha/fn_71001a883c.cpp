// Unit msapi::material::SAlpha: finds an entry by id (own entries first, then each group's) and copies
// up to 4 bytes into its data. Names and layouts are placeholders.
#include <algorithm>
#include <cstring>

#include "types.h"

struct UnkSAlphaEntry {
    u8 pad_0x0[8];
    u64 id;
    u8 pad_0x10[4];
    u32 words;
    u8 pad_0x18[0x18];
    void* data;
};

struct UnkSAlphaGroup {
    u8 pad_0x0[0x10];
    UnkSAlphaEntry* entries;
    u32 count;
    u8 pad_0x1c[0x44];
};

struct UnkSAlphaTable {
    u8 pad_0x0[0x330];
    UnkSAlphaEntry* entries;
    u32 count;
    u8 pad_0x33c[0x1c];
    UnkSAlphaGroup* groups;
    u32 groupCount;
};

static inline UnkSAlphaEntry* Find(UnkSAlphaEntry* entries, u32 count, u64 id) {
    return std::find_if(entries, entries + count, [id](const UnkSAlphaEntry& e) { return e.id == id; });
}

static UnkSAlphaEntry* Lookup(UnkSAlphaTable* self, const u64& id) {
    UnkSAlphaEntry* found = Find(self->entries, self->count, id);
    if (found != self->entries + self->count)
        return found;
    for (u32 i = 0; i < self->groupCount; ++i) {
        UnkSAlphaGroup& group = self->groups[i];
        found = Find(group.entries, group.count, id);
        if (found != group.entries + group.count)
            return found;
    }
    return nullptr;
}

bool fn_71001a883c(UnkSAlphaTable* self, const u64* id, const void* src) {
    UnkSAlphaEntry* entry = Lookup(self, *id);
    if (!entry)
        return false;
    s32 size = 0;
    if (entry->data) {
        size = entry->words * 4;
        if (size > 4)
            size = 4;
    }
    memcpy(entry->data, src, size);
    return true;
}

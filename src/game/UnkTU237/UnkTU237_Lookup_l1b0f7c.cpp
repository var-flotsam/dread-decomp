// Unit 237 (a table of indexed resources, no reliable name): looks up the resource of one kind and id in
// one entry (the default entry when none is given). Names and layouts are placeholders.
#include "types.h"

struct UnkResSlot_b0f7c {
    u8 pad_0x0[0x20];
    void** kind_2;  // 0x20
    void** kind_0;  // 0x28
    void** kind_1;  // 0x30
};
static_assert(sizeof(UnkResSlot_b0f7c) == 0x38, "UnkResSlot_b0f7c layout");

struct UnkResTable_b0f7c {
    u8 pad_0x0[0x18];
    s32 index_2[0x181];  // 0x18
    s32 index_0[0x181];  // 0x61c
    s32 index_1[0x181];  // 0xc20
    u8 pad_0x1224[0x4c];
    u32 entry_count;  // 0x1270
    s32 count_2;      // 0x1274
    s32 count_0;      // 0x1278
    s32 count_1;      // 0x127c
    u32 pad_0x1280;
    s32 default_entry;          // 0x1284
    UnkResSlot_b0f7c* entries;  // 0x1288
};
static_assert(__builtin_offsetof(UnkResTable_b0f7c, index_0) == 0x61c, "UnkResTable_b0f7c layout");
static_assert(__builtin_offsetof(UnkResTable_b0f7c, entry_count) == 0x1270, "UnkResTable_b0f7c layout");
static_assert(__builtin_offsetof(UnkResTable_b0f7c, entries) == 0x1288, "UnkResTable_b0f7c layout");

// Ids come in two ranges; the second starts at 0x2cd and follows the first in the index arrays.
static inline s32 IdToIndex(s32 id) {
    return (u32)id < 0x11b ? id : id - 0x2cd;
}

bool fn_71001b0f7c(UnkResTable_b0f7c* self, s32 kind, s32 id, void** out, s32 entry) {
    *out = nullptr;
    if (entry < 0)
        entry = self->default_entry;
    if (id == -1)
        return false;

    void* found = nullptr;
    if ((u32)entry < self->entry_count) {
        s32 i;
        switch (kind) {
        case 2:
            i = self->index_2[IdToIndex(id)];
            if (i >= 0 && i < self->count_2) {
                found = self->entries[entry].kind_2[i];
                *out = found;
            }
            break;
        case 1:
            i = self->index_1[IdToIndex(id)];
            if (i >= 0 && i < self->count_1) {
                found = self->entries[entry].kind_1[i];
                *out = found;
            }
            break;
        case 0:
            i = self->index_0[IdToIndex(id)];
            if (i >= 0 && i < self->count_0) {
                found = self->entries[entry].kind_0[i];
                *out = found;
            }
            break;
        }
    }
    return found != nullptr;
}

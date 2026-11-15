// Unit 270: puts a pool entry into (or takes it out of) the table of its kind and records its index there.
// Returns false for a null entry or one that is not from the pool's own array. Names and layouts are placeholders.
#include "types.h"

constexpr u32 kPoolSize_l1eae28 = 25000;

struct UnkPoolEntry_l1eae28 {
    u8 pad_0x0[0xd4];
    s32 kindSlot;  // 0xd4: index in its kind's table, or -1
    s32 kind;      // 0xd8
    u32 field_0xdc;
};
static_assert(sizeof(UnkPoolEntry_l1eae28) == 0xe0, "entry layout");

struct UnkPool_l1eae28 {
    void* vtable;
    UnkPoolEntry_l1eae28* slots[kPoolSize_l1eae28];
    UnkPoolEntry_l1eae28 entries[kPoolSize_l1eae28];  // 0x30d48
    UnkPoolEntry_l1eae28* tableA[kPoolSize_l1eae28];  // 0x588048: the entries of kind 0
    UnkPoolEntry_l1eae28* tableB[kPoolSize_l1eae28];  // 0x5b8d88
    u8 pad_0x5e9ac8[0xb4];
    u32 countA;  // 0x5e9b7c
    u32 countB;
    bool disabled;
    bool field_0x5e9b85;
    bool field_0x5e9b86;
    u8 changed;  // 0x5e9b87
};
static_assert(__builtin_offsetof(UnkPool_l1eae28, countA) == 0x5e9b7c, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1eae28, changed) == 0x5e9b87, "pool layout");

bool fn_71001eae28(UnkPool_l1eae28* self, UnkPoolEntry_l1eae28* entry, bool add) {
    if (!entry)
        return false;
    if (entry < self->entries || entry >= self->entries + kPoolSize_l1eae28)
        return false;
    s32 slot = entry->kindSlot;
    s32 kind = entry->kind;
    UnkPoolEntry_l1eae28** table = kind == 0 ? self->tableA : self->tableB;
    u32* count = kind == 0 ? &self->countA : &self->countB;
    if (add) {
        if (slot < 0) {
            self->changed = (kind == 0) | (self->changed != 0);
            entry->kindSlot = *count;
            table[*count] = entry;
            ++*count;
        }
    } else if (slot >= 0 && slot < (s32)*count) {
        self->changed = (kind == 0) | (self->changed != 0);
        u32 last = *count - 1;
        *count = last;
        s32 index = entry->kindSlot;
        UnkPoolEntry_l1eae28* moved = table[last];
        table[index] = moved;
        moved->kindSlot = index;
        table[*count] = entry;
        entry->kindSlot = -1;
    }
    return true;
}

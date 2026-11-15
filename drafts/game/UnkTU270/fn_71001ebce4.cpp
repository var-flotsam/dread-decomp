// Unit 270: takes every entry of one kind out of the pool, first one first, the way the single removal does:
// out of the table of its kind and out of the slot table, each by moving the last entry into its place.
// Names and layouts are placeholders.
#include "types.h"

constexpr u32 kPoolSize_l1ebce4 = 25000;

struct UnkPoolEntry_l1ebce4 {
    u8 pad_0x0[0xd0];
    s32 slot;      // 0xd0: index in the slot table, or -1
    s32 kindSlot;  // 0xd4: index in its kind's table, or -1
    s32 kind;      // 0xd8
    u32 field_0xdc;
};
static_assert(sizeof(UnkPoolEntry_l1ebce4) == 0xe0, "entry layout");

struct UnkPool_l1ebce4 {
    void* vtable;
    UnkPoolEntry_l1ebce4* slots[kPoolSize_l1ebce4];
    UnkPoolEntry_l1ebce4 entries[kPoolSize_l1ebce4];  // 0x30d48
    UnkPoolEntry_l1ebce4* tableA[kPoolSize_l1ebce4];  // 0x588048: the entries of kind 0
    UnkPoolEntry_l1ebce4* tableB[kPoolSize_l1ebce4];  // 0x5b8d88
    u8 pad_0x5e9ac8[0xac];
    u32 slotCount;          // 0x5e9b74
    u32 seenCount;          // 0x5e9b78
    u32 countA;             // 0x5e9b7c
    u32 countB;
    bool disabled;          // 0x5e9b84
    bool field_0x5e9b85;
    bool field_0x5e9b86;
    u8 changed;             // 0x5e9b87
};
static_assert(__builtin_offsetof(UnkPool_l1ebce4, tableA) == 0x588048, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1ebce4, slotCount) == 0x5e9b74, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1ebce4, changed) == 0x5e9b87, "pool layout");

// Takes the entry out of the pool, when it is one of the pool's and the pool takes changes.
static inline void RemoveEntry(UnkPool_l1ebce4* self, UnkPoolEntry_l1ebce4* entry) {
    if (!(!entry | self->disabled | (entry < self->entries) | (entry >= self->entries + kPoolSize_l1ebce4))) {
        if (self->slotCount == 0)
            return;
        s32 kindSlot = entry->kindSlot;
        s32 kind = entry->kind;
        UnkPoolEntry_l1ebce4** table = kind == 0 ? self->tableA : self->tableB;
        u32& count = kind == 0 ? self->countA : self->countB;
        if (kindSlot >= 0 && kindSlot < (s32)count) {
            self->changed = (kind == 0) | (self->changed != 0);
            u32 last = count - 1;
            count = last;
            s32 index = entry->kindSlot;
            UnkPoolEntry_l1ebce4* moved = table[last];
            table[index] = moved;
            moved->kindSlot = index;
            table[count] = entry;
            entry->kindSlot = -1;
        }
        u32 last = self->slotCount - 1;
        s32 index = entry->slot;
        self->slotCount = last;
        UnkPoolEntry_l1ebce4* moved = self->slots[last];
        self->slots[index] = moved;
        moved->slot = index;
        self->slots[self->slotCount] = entry;
        entry->slot = -1;
    }
}

void fn_71001ebce4(UnkPool_l1ebce4* self, s32 kind) {
    if (self->disabled)
        return;
    self->changed = true;
    UnkPoolEntry_l1ebce4** table = kind == 0 ? self->tableA : self->tableB;
    u32& count = kind == 0 ? self->countA : self->countB;
    if (count == 0)
        return;
    do {
        RemoveEntry(self, table[0]);
    } while (count != 0);
}

// Unit 270: takes a pool entry out of the pool: out of the table of its kind (when it is in one) and out of the
// slot table, each by moving the last entry into its place. Returns false when there is nothing to remove.
// Names and layouts are placeholders.
#include "types.h"

constexpr u32 kPoolSize_l1eba14 = 25000;

struct UnkPoolEntry_l1eba14 {
    u8 pad_0x0[0xd0];
    s32 slot;      // 0xd0: index in the slot table, or -1
    s32 kindSlot;  // 0xd4: index in its kind's table, or -1
    s32 kind;      // 0xd8
    u32 field_0xdc;
};
static_assert(sizeof(UnkPoolEntry_l1eba14) == 0xe0, "entry layout");

struct UnkPool_l1eba14 {
    void* vtable;
    UnkPoolEntry_l1eba14* slots[kPoolSize_l1eba14];
    UnkPoolEntry_l1eba14 entries[kPoolSize_l1eba14];  // 0x30d48
    UnkPoolEntry_l1eba14* tableA[kPoolSize_l1eba14];  // 0x588048: the entries of kind 0
    UnkPoolEntry_l1eba14* tableB[kPoolSize_l1eba14];  // 0x5b8d88
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
static_assert(__builtin_offsetof(UnkPool_l1eba14, tableA) == 0x588048, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1eba14, slotCount) == 0x5e9b74, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1eba14, changed) == 0x5e9b87, "pool layout");

bool fn_71001eba14(UnkPool_l1eba14* self, UnkPoolEntry_l1eba14* entry) {
    bool removed = false;
    bool inPool = !(entry < self->entries || entry >= self->entries + kPoolSize_l1eba14);
    if (entry && !self->disabled && inPool) {
        if (self->slotCount == 0)
            return false;
        s32 kindSlot = entry->kindSlot;
        s32 kind = entry->kind;
        UnkPoolEntry_l1eba14** table = kind == 0 ? self->tableA : self->tableB;
        u32& count = kind == 0 ? self->countA : self->countB;
        if (kindSlot >= 0 && kindSlot < (s32)count) {
            self->changed = (kind == 0) | (self->changed != 0);
            u32 last = count - 1;
            count = last;
            s32 index = entry->kindSlot;
            UnkPoolEntry_l1eba14* moved = table[last];
            table[index] = moved;
            moved->kindSlot = index;
            table[count] = entry;
            entry->kindSlot = -1;
        }
        u32 last = self->slotCount - 1;
        s32 index = entry->slot;
        self->slotCount = last;
        UnkPoolEntry_l1eba14* moved = self->slots[last];
        self->slots[index] = moved;
        moved->slot = index;
        self->slots[self->slotCount] = entry;
        entry->slot = -1;
        removed = true;
    }
    return removed;
}

// Unit 270: takes the pool entries standing for one object out of the pool, or every entry when the object is
// null (which only unlinks them from the slot table and empties it). Names and layouts are placeholders.
#include "types.h"

constexpr u32 kPoolSize_l1ebb78 = 25000;

struct UnkPoolEntry_l1ebb78 {
    void* object;  // 0x0: the object the entry stands for
    u8 pad_0x8[0xc8];
    s32 slot;      // 0xd0: index in the slot table, or -1
    s32 kindSlot;  // 0xd4: index in its kind's table, or -1
    s32 kind;      // 0xd8
    u32 field_0xdc;
};
static_assert(sizeof(UnkPoolEntry_l1ebb78) == 0xe0, "entry layout");

struct UnkPool_l1ebb78 {
    void* vtable;
    UnkPoolEntry_l1ebb78* slots[kPoolSize_l1ebb78];
    UnkPoolEntry_l1ebb78 entries[kPoolSize_l1ebb78];  // 0x30d48
    UnkPoolEntry_l1ebb78* tableA[kPoolSize_l1ebb78];  // 0x588048: the entries of kind 0
    UnkPoolEntry_l1ebb78* tableB[kPoolSize_l1ebb78];  // 0x5b8d88
    u8 pad_0x5e9ac8[0xac];
    u32 slotCount;  // 0x5e9b74
    u32 seenCount;  // 0x5e9b78
    u32 countA;     // 0x5e9b7c
    u32 countB;
    bool disabled;  // 0x5e9b84
    bool field_0x5e9b85;
    bool field_0x5e9b86;
    u8 changed;  // 0x5e9b87
};
static_assert(__builtin_offsetof(UnkPool_l1ebb78, slotCount) == 0x5e9b74, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1ebb78, changed) == 0x5e9b87, "pool layout");

void fn_71001ebb78(UnkPool_l1ebb78* self, void* object) {
    if (!self->disabled) {
        u32 n = self->slotCount;
        self->changed = true;
        if (!object) {
            for (u64 i = 0; i < n; ++i) {
                UnkPoolEntry_l1ebb78* entry = self->slots[i];
                entry->slot = -1;
                entry->kindSlot = -1;
                n = self->slotCount;
            }
            self->slotCount = 0;
            self->seenCount = 0;
            self->countA = 0;
        } else {
            for (u32 i = 0; i < n;) {
                UnkPoolEntry_l1ebb78* entry = self->slots[i];
                if (entry->object == object) {
                    if (entry && entry >= self->entries && entry < self->entries + kPoolSize_l1ebb78) {
                        s32 kindSlot = entry->kindSlot;
                        s32 kind = entry->kind;
                        UnkPoolEntry_l1ebb78** table = kind == 0 ? self->tableA : self->tableB;
                        u32& count = kind == 0 ? self->countA : self->countB;
                        if (kindSlot >= 0 && kindSlot < (s32)count) {
                            self->changed = (kind == 0) | (self->changed != 0);
                            u32 last = count - 1;
                            count = last;
                            s32 index = entry->kindSlot;
                            UnkPoolEntry_l1ebb78* moved = table[last];
                            table[index] = moved;
                            moved->kindSlot = index;
                            table[count] = entry;
                            entry->kindSlot = -1;
                        }
                    }
                    u32 last = self->slotCount - 1;
                    s32 index = entry->slot;
                    self->slotCount = last;
                    UnkPoolEntry_l1ebb78* moved = self->slots[last];
                    self->slots[index] = moved;
                    moved->slot = index;
                    self->slots[self->slotCount] = entry;
                    entry->slot = -1;
                    n = self->slotCount;
                } else {
                    ++i;
                }
            }
        }
    }
}

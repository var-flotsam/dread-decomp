// Removes the entry with the given id (-1 = none) from a keyed table: right away (after
// notifying fn_710070d5dc) unless the table is locked, in which case the id is queued.
#include "base/UnkVector.h"
#include "types.h"

// The engine's growable array (same layout as base::UnkVector). Its append copies the old
// elements one by one, reloading the count.
template <class T>
struct UnkArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void PushBack(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        T* p;
        if (old_capacity < new_capacity) {
            p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
            if (T* old = data) {
                T* src = old;
                T* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst)
                    __builtin_memcpy(dst, src, sizeof(T));
                allocator->vf_0x48(data);
            }
            data = p;
            n = count;
            capacity = new_capacity;
        } else {
            p = data;
        }
        __builtin_memcpy(&p[n], &value, sizeof(T));
        ++count;
    }
};

struct UnkTableEntry {
    u64 field_0x0;
    u8 unk_0x8[0x38];
};

// Values and keys in parallel arrays.
struct UnkKeyedTable {
    UnkTableEntry* values;
    u64* keys;
    u32 count;

    UnkTableEntry* Find(const u64& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
    UnkTableEntry* End() { return &values[count]; }
    u64 IndexOf(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return i;
        return count;
    }
};

struct UnkTableOwner {
    u8 unk_0x0[0x68];
    UnkKeyedTable table;
    u8 unk_0x80[0x10];
    bool locked;
    UnkArray<u8[0x10]> field_0x98;
    UnkArray<u64> pending_remove;
};

void fn_710070d5dc(u64 value);
void fn_7100a2d054(UnkKeyedTable* table, UnkTableEntry* entry);  // erase

// NON_MATCHING(minor): register allocation in the two lookups (the original keeps the found
// index and the count in different registers and materialises one more zero).
void fn_7100a2b7b8(UnkTableOwner* self, const u64* id) {
    u64 key = *id;
    if (key == (u64)-1)
        return;
    if (self->table.Find(key) == self->table.End())
        return;
    if (self->locked) {
        self->pending_remove.PushBack(*id);
        return;
    }
    u64 j = self->table.IndexOf(key);
    if (j != self->table.count) {
        UnkTableEntry* entry = &self->table.values[j];
        fn_710070d5dc(entry->field_0x0);
        fn_7100a2d054(&self->table, entry);
    }
}

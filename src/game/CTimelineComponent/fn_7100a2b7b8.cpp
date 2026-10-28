// Removes the entry with the given id (-1 = none) from a keyed table: right away (after
// notifying fn_710070d5dc) unless the table is locked, in which case the id is queued.
#include "base/UnkVector.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

// Appends to the engine's array, copying the old elements one by one and reloading the count.
template <class T>
static inline void AppendBitwise(base::UnkVector<T>& a, const T& value) {
    u32 n = a.count;
    u32 old_capacity = a.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && a.step != 0)
        new_capacity = (n / a.step + 1) * a.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    T* p;
    if (old_capacity < new_capacity) {
        p = static_cast<T*>(a.allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, a.flags));
        if (T* old = a.data) {
            T* src = old;
            T* dst = p;
            for (u32 i = 0; i < a.count; ++i, ++src, ++dst)
                __builtin_memcpy(dst, src, sizeof(T));
            a.allocator->vf_0x48(a.data);
        }
        a.data = p;
        n = a.count;
        a.capacity = new_capacity;
    } else {
        p = a.data;
    }
    __builtin_memcpy(&p[n], &value, sizeof(T));
    ++a.count;
}

void fn_710070d5dc(u64 value);
void fn_7100a2d054(UnkKeyedTable* table, UnkTableEntry* entry);  // erase

// NON_MATCHING(minor): register allocation in the two lookups (the original keeps the found
// index and the count in different registers and materialises one more zero).
void fn_7100a2b7b8(UnkTimelineComponent* self, const u64* id) {
    u64 key = *id;
    if (key == (u64)-1)
        return;
    if (self->table.Find(key) == self->table.End())
        return;
    if (self->locked) {
        AppendBitwise(self->pending_remove, *id);
        return;
    }
    u64 j = self->table.IndexOf(key);
    if (j != self->table.count) {
        UnkTableEntry* entry = &self->table.values[j];
        fn_710070d5dc(entry->field_0x0);
        fn_7100a2d054(&self->table, entry);
    }
}

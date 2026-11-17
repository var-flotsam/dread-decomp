// Unit 233 (tools/tu_map.py): on first use, resolves a table's relative entry pointer and indexes its
// entries in a small dictionary by key, keeping the first value of each key. Names and layouts are
// placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "types.h"

struct UnkKeyedEntry_l1ab9e4 {
    u16 key;
    u32 value;
};

// Turns an offset from `base`, stored in a pointer, into the pointer (null stays null).
template <class T>
inline void ResolveOffset(T*& ptr, const void* base) {
    if (ptr)
        ptr = reinterpret_cast<T*>(reinterpret_cast<u64>(ptr) + reinterpret_cast<u64>(base));
}

// A table loaded as one block: the entry pointer is stored as an offset from the table itself.
struct UnkKeyedTable_l1ab9e4 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkKeyedEntry_l1ab9e4* entries;

    void Relocate() { ResolveOffset(entries, this); }
};

struct UnkKeyedIndex_l1ab9e4 {
    bool built;
    u8 pad_0x1[0xf];
    UnkKeyedTable_l1ab9e4* table;
    base::global::CRntSmallDictionary<u32, u32> values;
};
static_assert(__builtin_offsetof(UnkKeyedIndex_l1ab9e4, values) == 0x18, "layout");

using UnkKeyedValues_l1ab9e4 = base::global::CRntSmallDictionary<u32, u32>;

// Appends an entry, growing the arrays by the step when they are full.
inline void Add(UnkKeyedValues_l1ab9e4& dict, const u32& key, const u32& value) {
    if (dict.count == dict.capacity)
        dict.Grow(dict.count + dict.step);
    dict.keys[dict.count] = key;
    dict.values[dict.count] = value;
    ++dict.count;
}

void fn_71001ab9e4(UnkKeyedIndex_l1ab9e4* self) {
    if (!self->table || self->built)
        return;
    self->table->Relocate();
    UnkKeyedTable_l1ab9e4* table = self->table;
    for (u64 i = 0; i < table->count; ++i) {
        const UnkKeyedEntry_l1ab9e4& entry = table->entries[i];
        u32 key = entry.key;
        if (self->values.Find(key) == self->values.End())
            Add(self->values, key, entry.value);
    }
    self->built = true;
}

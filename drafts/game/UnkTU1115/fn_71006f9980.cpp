// Unit 1115 (tools/tu_map.py, no class name): the registry deletes the object filed under an id, takes it
// out of its id index and files the remaining objects again. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// The registry and its entries as the unit's other functions take them (UnkUnit1115_b50.cpp).
struct UnkQueryEntry_b50;
struct UnkQueryList_b50;
void fn_71006facbc(UnkQueryEntry_b50* entry);
void fn_71006fb514(void* map, const u32* id, UnkQueryEntry_b50* const* entry);

// An object of the registry, deleted through its virtual destructor.
struct UnkRegObj_l6f9980 {
    virtual ~UnkRegObj_l6f9980();
    u32 id;  // 0x8
};

// A value of the index: an object pointer, held as an 8-byte value of unknown type (copied as bytes).
struct UnkIndexValue_l6f9980 {
    u8 bytes[8];

    UnkRegObj_l6f9980* Object() const { return *reinterpret_cast<UnkRegObj_l6f9980* const*>(bytes); }
};

// Parallel arrays of objects and their ids.
struct UnkIdIndex_l6f9980 {
    UnkIndexValue_l6f9980* values;  // 0x0
    u32* keys;                      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 step;                       // 0x18
    u32 flags;                      // 0x1c
    base::IAllocator* allocator;  // 0x20

    // The position of `key`, or the count when it is not filed.
    u64 IndexOf(u32 key) const {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return i;
        }
        return count;
    }

    UnkIndexValue_l6f9980* Find(u32 key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
    bool Contains(u32 key) { return Find(key) != values + count; }

    UnkIndexValue_l6f9980& operator[](u32 key) { return values[IndexOf(key)]; }

    // Takes the entry at `index` out, moving the last one into its place.
    void EraseAt(u32 index) {
        if (index != count - 1) {
            values[index] = values[count - 1];
            keys[index] = keys[count - 1];
        }
        count = count - 1;
    }

    void Erase(u32 key) {
        u64 index = IndexOf(key);
        if (index != count)
            EraseAt(index);
    }
};

struct UnkRegistry_l6f9980 {
    u8 pad_0x0[0x40];
    UnkIdIndex_l6f9980 index;                  // 0x40
    CRntVector<UnkRegObj_l6f9980*> objects;    // 0x68
};
static_assert(__builtin_offsetof(UnkRegistry_l6f9980, objects) == 0x68, "layout");

void fn_71006f9980(UnkQueryList_b50* registry, u32 id) {
    auto* self = reinterpret_cast<UnkRegistry_l6f9980*>(registry);
    if (!self->index.Contains(id))
        return;
    UnkRegObj_l6f9980* obj = self->index[id].Object();
    if (!obj)
        return;
    self->index.Erase(id);
    self->objects.Remove(obj);
    delete obj;

    self->index.count = 0;
    for (UnkRegObj_l6f9980 **p = self->objects.data, **end = p + self->objects.count; p != end; ++p) {
        UnkRegObj_l6f9980* entry = *p;
        fn_71006facbc(reinterpret_cast<UnkQueryEntry_b50*>(entry));
        u32 entry_id = entry->id;
        fn_71006fb514(&self->index, &entry_id, reinterpret_cast<UnkQueryEntry_b50* const*>(&entry));
    }
}

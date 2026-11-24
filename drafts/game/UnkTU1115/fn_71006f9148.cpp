// Unit 1115 (tools/tu_map.py, no class name): the registry deletes the object at an index, takes it out
// of its id index and files the remaining objects again. Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

struct UnkClone_fn_71006f9b48;
using UnkObjSlot_l6f9148 = UnkCloneElem<UnkClone_fn_71006f9b48, 8>;
extern template UnkObjSlot_l6f9148& CRntVector<UnkObjSlot_l6f9148>::GrowAt(u32);

// The entries as the unit's other functions take them (UnkUnit1115_b50.cpp).
struct UnkQueryEntry_b50;
void RefreshRegistryObjectGraphNodeIndices(UnkQueryEntry_b50* entry);
void AppendU32KeyU64ValueToSmallDictionary(void* map, const u32* id, UnkQueryEntry_b50* const* entry);

// An object of the registry, deleted through its virtual destructor.
struct UnkRegObj_l6f9148 {
    virtual ~UnkRegObj_l6f9148();
    u32 id;  // 0x8
};

// A value of the index: an object pointer, held as an 8-byte value of unknown type.
struct UnkIndexValue_l6f9148 {
    u8 bytes[8];
};

// Parallel arrays of objects and their ids.
struct UnkIdIndex_l6f9148 {
    UnkIndexValue_l6f9148* values;  // 0x0
    u32* keys;                      // 0x8
    u32 count;                      // 0x10
    u32 capacity;                   // 0x14
    u32 step;                       // 0x18
    u32 flags;                      // 0x1c
    base::UnkAllocator* allocator;  // 0x20

    UnkIndexValue_l6f9148* Find(u32 key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }

    // Takes the entry at index out, moving the last one into its place.
    void EraseAt(u32 index) {
        if (index != count - 1) {
            values[index] = values[count - 1];
            keys[index] = keys[count - 1];
        }
        count = count - 1;
    }

    void Erase(u32 key) {
        UnkIndexValue_l6f9148* it = Find(key);
        if (it != values + count)
            EraseAt(it - values);
    }
};

struct UnkRegistry_l6f9148 {
    u8 pad_0x0[0x40];
    UnkIdIndex_l6f9148 index;                      // 0x40
    CRntVector<UnkRegObj_l6f9148*> objects;        // 0x68
};
static_assert(__builtin_offsetof(UnkRegistry_l6f9148, objects) == 0x68, "layout");

void fn_71006f9148(void* registry, u32 index) {
    auto* self = static_cast<UnkRegistry_l6f9148*>(registry);
    if (index < self->objects.count) {
        // The element accessor is the out-of-line copy shared with other 8-byte arrays.
        auto* slots = reinterpret_cast<CRntVector<UnkObjSlot_l6f9148>*>(&self->objects);
        UnkRegObj_l6f9148* obj = *reinterpret_cast<UnkRegObj_l6f9148**>(&slots->GrowAt(index));
        self->index.Erase(obj->id);
        self->objects.Remove(obj);
        delete obj;

        self->index.count = 0;
        for (UnkRegObj_l6f9148 **p = self->objects.data, **end = p + self->objects.count; p != end; ++p) {
            UnkRegObj_l6f9148* entry = *p;
            RefreshRegistryObjectGraphNodeIndices(reinterpret_cast<UnkQueryEntry_b50*>(entry));
            u32 id = entry->id;
            AppendU32KeyU64ValueToSmallDictionary(&self->index, &id, reinterpret_cast<UnkQueryEntry_b50* const*>(&entry));
        }
    }
}

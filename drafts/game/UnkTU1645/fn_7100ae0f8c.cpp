// Unit 1645 (tools/tu_map.py, no reliable name): indexes every property on the global list by its
// group (a table per group, created when first seen, of the group's properties by id) and folds the
// properties' hashes into a checksum. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

using namespace base::global;

// As src/game/UnkTU1645/UnkTU1645_Reserve_ovb0261.cpp defines them (the map's Reserve takes them).
struct UnkReleasedId_b0261 {
    UnkStringData* data;

    UnkReleasedId_b0261(UnkReleasedId_b0261&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedId_b0261() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkKeyedSlot_b0261 {
    union {
        UnkKeyedStrIds* ptr;
    };
    UnkKeyedSlot_b0261(UnkKeyedSlot_b0261&& other) {
        UnkKeyedStrIds* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkKeyedSlot_b0261() {
        UnkKeyedStrIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ae2df8(old);
            fn_7100083c00(old);
        }
    }
};

using UnkKeyedMap_b0261 = base::global::CRntSmallDictionary<UnkReleasedId_b0261, UnkKeyedSlot_b0261>;

void fn_7100ae2ef4(UnkKeyedMap_b0261* self, u32 capacity);

// The group tables' Reserve (generated in src/clones/).
struct UnkClone_fn_7100ae3098;
using UnkGroupTable_lae0f8c = base::UnkCloneKeyedArray<UnkClone_fn_7100ae3098>;

// A property's group (vtable slot 0x20), its id (0x28) and its hash (0x40).
inline const CStrId* PropertyGroup(UnkPropertyHead* p) {
    return reinterpret_cast<const CStrId* (*)(UnkPropertyHead*)>(p->vtable[4])(p);
}
inline const CStrId* PropertyId(UnkPropertyHead* p) {
    return reinterpret_cast<const CStrId* (*)(UnkPropertyHead*)>(p->vtable[5])(p);
}
inline u32 PropertyHash(UnkPropertyHead* p) {
    return reinterpret_cast<u32 (*)(UnkPropertyHead*)>(p->vtable[8])(p);
}

// Copies an id into new storage, retaining its string; the pointer is stored bytewise, as the map's
// moves store it.
inline void CopyIdInto(void* slot, const CStrId& id) {
    UnkStringData* data = id.data;
    __builtin_memcpy(slot, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

// A group table's entry: the property (a union member, as the map's slots are).
struct UnkPropertySlot_lae0f8c {
    union {
        UnkPropertyHead* ptr;
    };
};

struct UnkPropertyIndex_lae0f8c {
    u8 pad_0x0[0x8];
    UnkKeyedMap_b0261 groups;  // a table of properties by id for each group
    u32 checksum;
};

void fn_7100ae0f8c(UnkPropertyIndex_lae0f8c* self) {
    self->checksum = ~0u;
    for (UnkPropertyHead* p = g_unk_71726db788; p; p = p->next) {
        UnkKeyedMap_b0261& map = self->groups;
        const CStrId* group = PropertyGroup(p);
        UnkKeyedSlot_b0261* slot = map.FindIf([&](const UnkReleasedId_b0261& id) { return group->data == id.data; });
        if (slot == map.End()) {
            const CStrId* name = PropertyGroup(p);
            auto* table = static_cast<UnkKeyedStrIds*>(fn_7100083b60(sizeof(UnkKeyedStrIds)));
            table->keys = nullptr;
            table->ids = nullptr;
            table->count = 0;
            table->capacity = 0;
            table->field_0x18 = 10;
            table->field_0x1c = 0xe;
            table->allocator = GetDefaultAllocator();

            if (map.count == map.capacity)
                fn_7100ae2ef4(&map, map.count + map.step);
            CopyIdInto(&map.keys[map.count], *name);
            map.values[map.count].ptr = table;
            u32 n = ++map.count;
            UnkKeyedSlot_b0261* end = &map.values[n];
            slot = n ? &end[-1] : end;
        }

        UnkKeyedStrIds* table = slot->ptr;
        const CStrId* id = PropertyId(p);
        if (table->count == table->capacity)
            reinterpret_cast<UnkGroupTable_lae0f8c*>(table)->Reserve(table->count + table->field_0x18);
        CopyIdInto(&table->ids[table->count], *id);
        static_cast<UnkPropertySlot_lae0f8c*>(table->keys)[table->count].ptr = p;
        ++table->count;
        self->checksum ^= PropertyHash(p);
    }
}

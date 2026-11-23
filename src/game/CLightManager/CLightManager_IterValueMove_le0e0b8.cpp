// CLightManager unit: the move assignment of the iterator value over the map of lights keyed by file-path
// ids (the values of src/game/CLightManager/CLightManager_Lists_ovb0060.cpp: a 0x58-byte base, a
// file-path id, an array, a flag and a word). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CLightsDef.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// Releases the id as its destructor does (in two steps), leaving the empty id; returns the pool.
static inline UnkStringPool* ReleasePath_le0e0b8(CFilePathStrId& id) {
    UnkStringPool* pool = g_unk_7101d47200;
    if (pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                StringPool_Release(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
        id.data = &pool->empty;
    }
    return pool;
}

// The key's move: the moved-from key gets the empty id through a store with no type-based alias info,
// so the holder's kind is loaded again after it, as the original does.
static inline void TakePathReloading_le0e0b8(CFilePathStrId& id, CFilePathStrId& other) {
    ReleasePath_le0e0b8(id);
    id.data = other.data;
    union Slot {
        UnkStringData* p;
    };
    reinterpret_cast<Slot*>(&other.data)->p = &g_unk_7101d47200->empty;
}

struct UnkLightArray_le0e0b8 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    void* allocator;

    void MoveFrom(UnkLightArray_le0e0b8& other) {
        if (this != &other) {
            fn_7100da0870(reinterpret_cast<UnkClearedArray*>(this), 0);
            allocator = other.allocator;
            flags = other.flags;
            step = other.step;
            field_0x14 = other.field_0x14;
            count = other.count;
            capacity = other.capacity;
            data = other.data;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
    }
};

struct UnkLightValue_le0e0b8 {
    u8 base_0x0[0x58];
    CFilePathStrId path;          // 0x58
    UnkLightArray_le0e0b8 array;  // 0x60
    bool field_0x88;
    u32 field_0x8c;

    // Takes the other's array, then its path (the other keeps the empty one), flag and word.
    UnkLightValue_le0e0b8& operator=(UnkLightValue_le0e0b8&& other) {
        array.MoveFrom(other.array);
        UnkStringPool* pool = ReleasePath_le0e0b8(path);
        path.data = other.path.data;
        other.path.data = &pool->empty;
        field_0x88 = other.field_0x88;
        field_0x8c = other.field_0x8c;
        return *this;
    }
};
static_assert(sizeof(UnkLightValue_le0e0b8) == 0x90, "UnkLightValue_le0e0b8 layout");

using UnkLightIterValue_le0e0b8 = base::UnkIterValue<UnkLightValue_le0e0b8, CFilePathStrId>;

// Moves the (path, light) pair `other` holds or refers to into the one `self` holds or refers to.
UnkLightIterValue_le0e0b8* fn_7100e0e0b8(UnkLightIterValue_le0e0b8* self, UnkLightIterValue_le0e0b8* other) {
    if (other->state == 1) {
        TakePathReloading_le0e0b8(self->Index(), other->value.index);
        self->Element() = static_cast<UnkLightValue_le0e0b8&&>(other->value.element);
    } else if (other->state == 2) {
        CFilePathStrId& key = *const_cast<CFilePathStrId*>(other->ref.index);
        TakePathReloading_le0e0b8(self->Index(), key);
        self->Element() = static_cast<UnkLightValue_le0e0b8&&>(*other->ref.element);
    }
    return self;
}

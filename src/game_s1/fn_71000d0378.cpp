// Clears of arrays of string ids (destroying the ids, keeping the storage), as fn_710104f600
// (src/game_s7/fn_710104f600.cpp) does for ids with two values: ids in CFilePathStrId's pool
// (fn_71000d0378, fn_71000cdbbc), and ids at +0x8 of their owner, destroyed only when the array
// has storage (fn_71000deb68, fn_7100251758). Also move-assignments of arrays of ids in
// CFilePathStrId's pool, as fn_710104ee34 for ids with two values (fn_71000d0860, fn_71000ce0a4).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
template <UnkStringPool** Pool>
struct UnkReleasedStrIdT {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkReleasedStrIdT(UnkReleasedStrIdT&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &(*Pool)->empty;
    }
    ~UnkReleasedStrIdT() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

using UnkReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47108>;
// The same id in CFilePathStrId's pool.
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;

template <class T>
struct UnkClearedArray {
    T* data;
    u32 count;

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
    }
};

struct UnkIdArrayOwner {
    u64 field_0x0;
    UnkClearedArray<UnkReleasedStrId> ids;
};

void fn_71000d0378(UnkClearedArray<UnkFilePathReleasedStrId>* self) { self->Clear(); }

void fn_71000cdbbc(UnkClearedArray<UnkFilePathReleasedStrId>* self) { self->Clear(); }

void fn_71000deb68(UnkIdArrayOwner* self) {
    UnkClearedArray<UnkReleasedStrId>* ids = &self->ids;
    if (ids->data) {
        for (u64 i = 0; i < ids->count; ++i)
            ids->data[i].~UnkReleasedStrId();
    }
    ids->count = 0;
}

void fn_7100251758(UnkIdArrayOwner* self) {
    UnkClearedArray<UnkReleasedStrId>* ids = &self->ids;
    if (ids->data) {
        for (u64 i = 0; i < ids->count; ++i)
            ids->data[i].~UnkReleasedStrId();
    }
    ids->count = 0;
}

// Move-assignment of an array: the old ids are released (the storage is kept), the other's array
// is taken and left empty.
template <class T>
struct UnkMovedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkMovedArray* src) {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
        allocator = src->allocator;
        flags = src->flags;
        step = src->step;
        field_0x14 = src->field_0x14;
        count = src->count;
        capacity = src->capacity;
        data = src->data;
        src->data = nullptr;
        src->count = 0;
        src->capacity = 0;
    }
};

using UnkFilePathIdArray = UnkMovedArray<UnkFilePathReleasedStrId>;

UnkFilePathIdArray* fn_71000d0860(UnkFilePathIdArray* self, UnkFilePathIdArray* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

UnkFilePathIdArray* fn_71000ce0a4(UnkFilePathIdArray* self, UnkFilePathIdArray* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

// Move-assignment of a counted array of a file path id with three values, as fn_710104f9e4
// (src/game/CFXPool/CDefinition/fn_710104ee34.cpp): fn_7100ffb9d8.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
template <UnkStringPool** Pool>
struct UnkReleasedStrIdT {
    UnkStringData* data;

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
// The id in CFilePathStrId's pool.
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;

struct UnkIdValues {
    UnkFilePathReleasedStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u64 field_0x10;
};

template <class T>
struct UnkMovedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    template <class Index = u64>
    __attribute__((always_inline)) void MoveFrom(UnkMovedArray* src) {
        for (Index i = 0; i < count; ++i)
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

UnkMovedArray<UnkIdValues>* fn_7100ffb9d8(UnkMovedArray<UnkIdValues>* self, UnkMovedArray<UnkIdValues>* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

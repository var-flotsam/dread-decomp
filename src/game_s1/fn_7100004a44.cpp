// The destructor of fn_7100e4a1d0 (src/game_aca57c/fn_7100e4fd1c.cpp) for a 0x38-byte record without
// the leading words: two string ids and an array of string ids (base::UnkVector's layout):
// fn_7100004a44.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkReleasedStrIdArray {
    UnkReleasedStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkReleasedStrIdArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkReleasedStrId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkIdsRecord38 {
    UnkReleasedStrId id_0x0;
    UnkReleasedStrId id_0x8;
    UnkReleasedStrIdArray ids;

    __attribute__((always_inline)) ~UnkIdsRecord38() {}
};
static_assert(sizeof(UnkIdsRecord38) == 0x38, "UnkIdsRecord38 layout");

void fn_7100004a44(UnkIdsRecord38* self) { self->~UnkIdsRecord38(); }


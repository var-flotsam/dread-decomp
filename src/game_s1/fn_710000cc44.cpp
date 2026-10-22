// Move-assignment of a string id, as fn_7100ead030 (src/game_aca57c/fn_7100d3d424.cpp) does for an
// id with two values: unless assigning to itself, the old id is released and the other's taken
// (leaving the pool's empty string): fn_710000cc44, fn_710000ba0c.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkReleasedStrId(UnkReleasedStrId&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedStrId() {
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

UnkReleasedStrId* fn_710000cc44(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    if (self != other) {
        self->~UnkReleasedStrId();
        new (self) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*other));
    }
    return self;
}

UnkReleasedStrId* fn_710000ba0c(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    if (self != other) {
        self->~UnkReleasedStrId();
        new (self) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*other));
    }
    return self;
}

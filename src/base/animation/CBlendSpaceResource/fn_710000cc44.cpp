// Move-assignment of a string id, as fn_7100ead030 (src/game/logic/collision/CCollider/fn_7100d3d424.cpp) does for an
// id with two values: unless assigning to itself, the old id is released and the other's taken
// (leaving the pool's empty string): in CFilePathStrId's pool (fn_710000cc44) or the default one
// (fn_710000ba0c).
#include <new>

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
using UnkFilePathReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47200>;

template <class T>
static inline T* MoveAssign(T* self, T* other) {
    if (self != other) {
        self->~T();
        new (self) T(static_cast<T&&>(*other));
    }
    return self;
}

UnkFilePathReleasedStrId* fn_710000cc44(UnkFilePathReleasedStrId* self, UnkFilePathReleasedStrId* other) {
    return MoveAssign(self, other);
}

UnkReleasedStrId* fn_710000ba0c(UnkReleasedStrId* self, UnkReleasedStrId* other) { return MoveAssign(self, other); }

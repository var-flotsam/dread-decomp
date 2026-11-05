// Copies of fn_71000cff64 (src/base/reflection/UnkUnit83/UnkPathByIdIterMove_b60.cpp): move-assignment
// of the (u64 index, string id) pair another iterator value holds or refers to, without a self check.
// fn_7100edaebc (unit 2576, CShotManager::CTunableShotManager) on a file path id; fn_7100fb0d68
// (unit 2612) on a string id.

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The index: its store may alias the holder's state (the original reloads the state after it).
typedef u64 __attribute__((may_alias)) UnkIndex_b61;


// A string id released in two steps (decrement, then reload and test), in the given pool.
template <UnkStringPool** Pool>
struct UnkPoolId_b61 {
    UnkStringData* data;

    UnkPoolId_b61& operator=(UnkPoolId_b61&& other) {
        UnkStringPool* pool = *Pool;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &(*Pool)->empty;
        return *this;
    }
};


// The layout of base::UnkIterValue<T, u64> (base/UnkCollectionIter.h), with an aliasing index.
template <class T>
struct UnkIdIterValue_b61 {
    union {
        struct {
            UnkIndex_b61 index;
            T element;
        } value;
        struct {
            const UnkIndex_b61* index;
            T* element;
        } ref;
    };
    s32 state;

    UnkIndex_b61& Index() { return state == 2 ? *const_cast<UnkIndex_b61*>(ref.index) : value.index; }
    T& Element() { return state == 2 ? *ref.element : value.element; }
};

template <class T>
inline __attribute__((always_inline)) UnkIdIterValue_b61<T>* MoveAssign_b61(UnkIdIterValue_b61<T>* self,
                                                                           UnkIdIterValue_b61<T>* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<T&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<T&&>(*other->ref.element);
    }
    return self;
}

using UnkPathIterValue_b61 = UnkIdIterValue_b61<UnkPoolId_b61<&g_unk_7101d47200>>;
using UnkStrIterValue_b61 = UnkIdIterValue_b61<UnkPoolId_b61<&g_unk_7101d47108>>;

UnkPathIterValue_b61* fn_7100edaebc(UnkPathIterValue_b61* self, UnkPathIterValue_b61* other) {
    return MoveAssign_b61(self, other);
}

UnkStrIterValue_b61* fn_7100fb0d68(UnkStrIterValue_b61* self, UnkStrIterValue_b61* other) {
    return MoveAssign_b61(self, other);
}

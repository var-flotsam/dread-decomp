// Unit 83 (base::reflection): slot 0x50 of the key-element iterator types of
// CRntSmallDictionary<CStrId, CFilePathStrId> (fn_71000c5868; fn_71000c5d44 is the same code):
// a cached iterator value (base::UnkIterValue keyed by a string id, holding a file path id) is
// reset, then takes the other's state and its 16 bytes bitwise.

#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id released in two steps (decrement, then reload and test), in the given pool.
template <UnkStringPool** Pool>
struct UnkPoolId_b60 {
    UnkStringData* data;

    // Releases the id, then takes the other's, leaving it the pool's empty string.
    UnkPoolId_b60& operator=(UnkPoolId_b60&& other) {
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
    ~UnkPoolId_b60() {
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

using UnkPathByIdIterValue_b60 =
    base::UnkIterValue<UnkPoolId_b60<&g_unk_7101d47200>, UnkPoolId_b60<&g_unk_7101d47108>>;

static inline __attribute__((always_inline)) UnkPathByIdIterValue_b60* MoveIterValue(
    UnkPathByIdIterValue_b60* self, const UnkPathByIdIterValue_b60* other) {
    if (self != other) {
        // base::UnkIterValue::Reset, inlined.
        if (self->state == 2) {
            self->state = 0;
        } else if (self->state == 1) {
            self->value.~Value();
            self->state = 0;
        }
        self->state = other->state;
        __builtin_memcpy(&self->ref, &other->ref, sizeof(self->ref));
    }
    return self;
}

__attribute__((flatten)) UnkPathByIdIterValue_b60* fn_71000c5868(UnkPathByIdIterValue_b60* self,
                                                                 const UnkPathByIdIterValue_b60* other) {
    return MoveIterValue(self, other);
}

__attribute__((flatten)) UnkPathByIdIterValue_b60* fn_71000c5d44(UnkPathByIdIterValue_b60* self,
                                                                 const UnkPathByIdIterValue_b60* other) {
    return MoveIterValue(self, other);
}

// Move-assignment of the pair another value holds or refers to, without a self check
// (fn_71000cd7a8 on a string id by u32 index; fn_71000cff64 on a file path id).
template <class T, class K>
static inline __attribute__((always_inline)) base::UnkIterValue<T, K>* MoveAssign(
    base::UnkIterValue<T, K>* self, base::UnkIterValue<T, K>* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = static_cast<T&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = static_cast<T&&>(*other->ref.element);
    }
    return self;
}

using UnkIdIterValue_b60 = base::UnkIterValue<UnkPoolId_b60<&g_unk_7101d47108>, u32>;
using UnkPathIterValue_b60 = base::UnkIterValue<UnkPoolId_b60<&g_unk_7101d47200>, u32>;

__attribute__((flatten)) UnkIdIterValue_b60* fn_71000cd7a8(UnkIdIterValue_b60* self, UnkIdIterValue_b60* other) {
    return MoveAssign(self, other);
}

__attribute__((flatten)) UnkPathIterValue_b60* fn_71000cff64(UnkPathIterValue_b60* self,
                                                             UnkPathIterValue_b60* other) {
    return MoveAssign(self, other);
}

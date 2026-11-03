// Unit 25: grows an array of id and path pairs (the words of base/UnkStrIdPathPair.h; the
// base::UnkVector layout) to a new capacity, moving the pairs over and destroying the old ones.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// The pair's two ids share one base holding the string (so a store to one pair's path may
// alias another's id: the original reloads the moved-from id after the new pair's path is
// written), and the move's store to the new id is untyped (the original reloads the array's
// count on every pass). Moves and releases as base::UnkStrIdMemberOf.
struct UnkIdData_b45a1 {
    UnkStringData* data;
};

template <base::global::UnkStringPool*& Pool>
struct UnkMovedId_b45a1 : UnkIdData_b45a1 {
    UnkMovedId_b45a1(UnkMovedId_b45a1&& other) {
        __builtin_memcpy(&data, &other.data, sizeof(data));
        other.data = &Pool->empty;
    }
    ~UnkMovedId_b45a1() {
        if (base::global::UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMovedPair_b45a1 {
    UnkMovedId_b45a1<g_unk_7101d47108> id;
    UnkMovedId_b45a1<g_unk_7101d47200> path;
};

struct UnkStrIdPathPairVec_b45a1 {
    UnkMovedPair_b45a1* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710001e4e8(UnkStrIdPathPairVec_b45a1* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkMovedPair_b45a1*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkMovedPair_b45a1), 8, self->flags));
        if (UnkMovedPair_b45a1* src = self->data) {
            UnkMovedPair_b45a1* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkMovedPair_b45a1(static_cast<UnkMovedPair_b45a1&&>(*src));
                src->~UnkMovedPair_b45a1();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

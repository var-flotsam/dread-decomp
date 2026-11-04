// Grows an array of (string id, counted resource reference) pairs to `capacity` elements:
// base::UnkVector's Reserve written out, as fn_71000231f4 (src/base/UnkUnit27/
// UnkCountedRefVector_Reserve_b53.cpp) with a string id in front of the reference. Two copies:
// fn_71001d7b64 (unit 259) over the string pool and fn_71002682e8 (unit 351,
// navmesh::CDynamicSmartLinkGroup) over the file-path pool. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id on `*Pool` that moves by taking the other's string (the other gets the empty one)
// and is released in two steps (decrement, then reload and test), as CStrId::Reset.
template <UnkStringPool** Pool>
struct UnkMovedId_b58 {
    UnkStringData* data;

    UnkMovedId_b58(UnkMovedId_b58&& other) {
        data = other.data;
        other.data = &(*Pool)->empty;
    }
    ~UnkMovedId_b58() {
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

// As UnkCountedRef_b53.
struct UnkCountedRef_b58 {
    UnkCountedResource* ptr;

    UnkCountedRef_b58(UnkCountedRef_b58&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkCountedRef_b58() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

template <UnkStringPool** Pool>
struct UnkNamedRef_b58 {
    UnkMovedId_b58<Pool> name;
    UnkCountedRef_b58 ref;
};

template <UnkStringPool** Pool>
__attribute__((always_inline)) inline void Reserve(base::UnkVector<u8>* vector, u32 capacity) {
    using T = UnkNamedRef_b58<Pool>;
    auto* self = reinterpret_cast<base::UnkVector<T>*>(vector);
    if (self->capacity < capacity) {
        auto* p = static_cast<T*>(self->allocator->vf_0x40((u64)capacity * sizeof(T), 8, self->flags));
        if (self->data) {
            T* src = self->data;
            T* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) T(static_cast<T&&>(*src));
                src->~T();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

}  // namespace

void fn_71001d7b64(base::UnkVector<u8>* vector, u32 capacity) {
    Reserve<&g_unk_7101d47108>(vector, capacity);
}

void fn_71002682e8(base::UnkVector<u8>* vector, u32 capacity) {
    Reserve<&g_unk_7101d47200>(vector, capacity);
}

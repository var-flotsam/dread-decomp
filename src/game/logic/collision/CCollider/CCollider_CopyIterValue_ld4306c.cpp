// game::logic::collision::CCollider unit: copy assignment of a reflected iterator value
// (base/UnkCollectionIter.h) with a u32 index and a growable array of 8-byte elements (base::global::CRntVector's
// layout): the array grows to the source's capacity when it is smaller and copies the elements.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkWordArray {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkWordArray& operator=(const UnkWordArray& other) {
        if (this != &other) {
            count = 0;
            u32 wanted = other.capacity;
            if (capacity < wanted) {
                u64* p = static_cast<u64*>(allocator->vf_0x40((u64)wanted * sizeof(u64), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = wanted;
            }
            __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u64));
            count = other.count;
        }
        return *this;
    }
};

void* fn_7100d4306c(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkWordArray, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}

// CEmmyAIComponent unit (tools/tu_map.py unit 811): appends a copy of an inner array to an array of
// arrays, the same code as fn_7100d43e70 (src/game/CCollider/CCollider_CopyArrays.cpp); its caller is
// fn_71006fb3f4 (unit 1115). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// An inner array: base::UnkVector's layout with a move that leaves the source empty.
struct UnkInnerVec {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkInnerVec(UnkInnerVec&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    // Copies another inner array of 4-byte elements: the capacity reserved, then the elements.
    UnkInnerVec(const UnkInnerVec& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (n != 0) {
            void* p = allocator->vf_0x40((u64)n * 4, 8, flags);
            if (data) {
                __builtin_memcpy(p, data, (u64)count * 4);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * 4);
        count = other.count;
    }
};

typedef base::UnkVector<UnkInnerVec> UnkOuterVec;

void fn_710056041c(UnkOuterVec* self, const UnkInnerVec* value) {
    self->PushBackCopy(*value);
}

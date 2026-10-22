// Copies of fn_7100d43c6c (src/game_aca57c/fn_7100d3d424.cpp) for another element type, naming their own callees and
// data (the comments are adapted from the original's): fn_710011eff4, fn_7100125584. Move-assignments of arrays of
// records holding an array (which frees its storage) and 0x20 more bytes: the old elements are destroyed (the storage
// is kept), the other's array is taken and left empty.
#include "base/UnkVector.h"
#include "types.h"

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

// The data pointer is a union member (no type-based alias info: it is reloaded after the
// element's other fields are moved).
template <class T>
struct UnkFreedVector {
    union {
        T* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkFreedVector() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkFreedVectorRecord {
    UnkFreedVector<u32> array;
    u64 unk_0x28[4];
};

UnkMovedArray<UnkFreedVectorRecord>* fn_710011eff4(UnkMovedArray<UnkFreedVectorRecord>* self,
                                                   UnkMovedArray<UnkFreedVectorRecord>* other) {
    if (self != other)
        self->MoveFrom(other);
    return self;
}

// The same array with a flag after it, taken last.
template <class T>
struct UnkFlaggedMovedArray : UnkMovedArray<T> {
    bool field_0x28;
    u8 unk_0x29[0xf];
    bool field_0x38;
};

UnkFlaggedMovedArray<UnkFreedVectorRecord>* fn_7100125584(UnkFlaggedMovedArray<UnkFreedVectorRecord>* self,
                                                          UnkFlaggedMovedArray<UnkFreedVectorRecord>* other) {
    if (self != other) {
        self->MoveFrom(other);
        self->field_0x38 = other->field_0x38;
    }
    return self;
}

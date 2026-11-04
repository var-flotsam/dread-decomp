// base::reflection collection slot 0x128 (src/reflection_types/collection_types_00.cpp) for (data, count)
// arrays of trivially copyable 2-, 4- and 8-byte elements: copies of src/base/reflection/UnkArrayEraseAt_A6_b68.cpp
// (fn_7100eb22d0 for 4-byte elements, fn_7100b83f80 for 8-byte ones; fn_71000d15fc has 2-byte ones) in
// this address range. Names are placeholders.
#include "types.h"

template <typename T>
struct UnkPodArray_A1_b72 {
    T* data;
    u32 count;
};

template <typename T>
static inline __attribute__((always_inline)) void EraseAtIndex_A1_b72(UnkPodArray_A1_b72<T>* self, u32 index) {
    if (index < self->count) {
        T* pos = self->data + index;
        __builtin_memcpy(pos, pos + 1, (self->count - index - 1) * sizeof(T));
        --self->count;
    }
}

template <typename T>
static inline __attribute__((always_inline)) void EraseAtIter_A1_b72(UnkPodArray_A1_b72<T>* self, const u32* it) {
    T* base = reinterpret_cast<T*>(self);
    T* p = reinterpret_cast<T*>(reinterpret_cast<char*>(base) + (u64)*it * sizeof(T));
    EraseAtIndex_A1_b72(self, (u32)(p - base));
}

#define ERASE_AT_A1_B72(addr, T) \
    void fn_##addr(UnkPodArray_A1_b72<T>* self, const u32* it) { EraseAtIter_A1_b72(self, it); }

ERASE_AT_A1_B72(71000d2514, u32)
ERASE_AT_A1_B72(71000d4328, u32)
ERASE_AT_A1_B72(71000d3410, u64)
ERASE_AT_A1_B72(710013f5cc, u64)
ERASE_AT_A1_B72(7100204e50, u64)
ERASE_AT_A1_B72(710023efb4, u64)
ERASE_AT_A1_B72(710026bd8c, u64)
ERASE_AT_A1_B72(71000d15fc, u16)

// In the CCharClassRodotukAIComponent unit: the copy constructor (fn_7100670fe4) and the copy
// assignment (fn_7100671130) of a reflected array of 0x30-byte trivially copyable entries (the
// UnkVector layout: data, count, capacity, step, a flag, the allocator's flags and the allocator).
// Both size the array to the source's capacity and copy the entries bitwise. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

namespace {
template <u32 Size>
struct UnkPodEntry {
    u8 bytes[Size];
};

template <class T>
struct UnkPodVector {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) T* Grow(u32 cap) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)cap * sizeof(T), 8, flags));
        if (data) {
            __builtin_memcpy(p, data, (u64)count * sizeof(T));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = cap;
        return p;
    }

    __attribute__((always_inline)) void CopyConstruct(const UnkPodVector& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        u32 cap = other.capacity;
        T* p = cap != 0 ? Grow(cap) : nullptr;
        __builtin_memcpy(p, other.data, (u64)other.count * sizeof(T));
        count = other.count;
    }

    __attribute__((always_inline)) UnkPodVector& Assign(const UnkPodVector& other) {
        if (this != &other) {
            count = 0;
            u32 cap = other.capacity;
            T* p = capacity < cap ? Grow(cap) : data;
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(T));
            count = other.count;
        }
        return *this;
    }
};

using UnkRodotukVector = UnkPodVector<UnkPodEntry<0x30>>;
}  // namespace

void fn_7100670fe4(void* dst, const void* src) {
    static_cast<UnkRodotukVector*>(dst)->CopyConstruct(*static_cast<const UnkRodotukVector*>(src));
}
void* fn_7100671130(void* dst, const void* src) {
    return &static_cast<UnkRodotukVector*>(dst)->Assign(*static_cast<const UnkRodotukVector*>(src));
}

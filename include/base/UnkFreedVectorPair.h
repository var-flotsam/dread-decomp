#pragma once

// The 0x60-byte record UnkFreedVectorPair (two vectors that free their storage when destroyed,
// and four words) and the arrays of it: UnkMovedArray (moved element by element), UnkPairArray (its
// Reserve is out of line, fn_7100e52b58) and UnkSecondPtrArray (a second pointer array alongside;
// its Destroy is out of line, fn_7100e493f4). Both functions are defined in
// src/game_aca57c/fn_7100d3d424.cpp. The element types are placeholders: only their move
// constructors and destructors matter here.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

// An array in base::UnkVector's layout with its Reserve and MoveFrom inlined: MoveFrom destroys
// the elements (the storage is kept), then takes the other's array and leaves it empty.
template <class T>
struct UnkMovedArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Grows the array, moving the elements over (fn_7100e52b58 for array pairs).
    __attribute__((always_inline)) void Reserve(u32 new_capacity) {
        if (capacity < new_capacity) {
            T* p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
            if (data) {
                T* src = data;
                T* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst) {
                    new (dst) T(static_cast<T&&>(*src));
                    src->~T();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = new_capacity;
        }
    }

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

// A vector that frees its storage when destroyed, so arrays of arrays free each element.
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

    // Takes over the other's array, leaving it empty.
    UnkFreedVector(UnkFreedVector&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkFreedVector() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkFreedVectorPair {
    UnkFreedVector<u32> first;
    UnkFreedVector<u32> second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    UnkFreedVectorPair(UnkFreedVectorPair&& other)
        : first(static_cast<UnkFreedVector<u32>&&>(other.first)),
          second(static_cast<UnkFreedVector<u32>&&>(other.second)), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
};

// An array with a second pointer array alongside, which Destroy frees with it.
template <class T>
struct UnkSecondPtrArray {
    T* data;
    void* field_0x8;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkSecondPtrArray(UnkSecondPtrArray&& other)
        : data(other.data), field_0x8(other.field_0x8), count(other.count), capacity(other.capacity),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.field_0x8 = nullptr;
    }

    // Destroys the elements and frees both arrays (fn_7100e493f4 for array pairs).
    __attribute__((always_inline)) void Destroy() {
        if (data)
            for (u32 i = 0; i < count; ++i)
                data[i].~T();
        count = 0;
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (data)
            allocator->vf_0x48(data);
    }
};

// An array of array pairs.
struct UnkPairArray : UnkMovedArray<UnkFreedVectorPair> {};

// The array's Reserve: moves the pairs into a new array.
void fn_7100e52b58(UnkPairArray* self, u32 capacity);

// Destroys the elements and frees both arrays.
void fn_7100e493f4(UnkSecondPtrArray<UnkFreedVectorPair>* self);
void fn_7100e52cf4(UnkPairArray* self, const UnkFreedVectorPair* value);

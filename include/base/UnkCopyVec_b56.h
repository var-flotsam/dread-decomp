#pragma once

// A growable array with base::global::CRntVector's layout whose Reserve stays out of line: each element type
// specialises Reserve to call its own copy (unit 2601's fn_7100f86020 and fn_7100f85d24, the music manager's
// path list's fn_7100456980). The copy constructor copies the settings, reserves the other's capacity and
// appends each element with PushBack's growth. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

template <class T>
struct UnkCopyVec_b56 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n);

    __attribute__((always_inline)) void PushBackCopy(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) T(value);
        ++count;
    }

    __attribute__((always_inline)) void AppendAll(const UnkCopyVec_b56& other) {
        Reserve(other.capacity);
        for (const T *p = other.data, *e = p + other.count; p != e; ++p)
            PushBackCopy(*p);
    }

    // Destroys the elements and releases the storage (base::global::CRntVector::Free).
    void Free() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    __attribute__((always_inline)) void CopyFrom(const UnkCopyVec_b56& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(other.capacity);
        for (const T *p = other.data, *e = p + other.count; p != e; ++p)
            PushBackCopy(*p);
    }
};

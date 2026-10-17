#pragma once

// The engine's growable array and the allocator interface it allocates through. Neither name
// is known; both keep placeholders. The layout is shared by CType+0x90 (default-constructed
// there: growth step 10, flags 0x3e) and the parameter-type list of a reflected function
// (UnkReflFunction+0x8, filled by fn_7100a19530).

#include "types.h"

namespace base {

// Only the slots the array uses are named. vf_0x40 allocates (size, alignment, flags);
// vf_0x48 frees.
struct UnkAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
    virtual void vf_0x48(void* ptr);
};

}  // namespace base

// Returns the allocator new arrays are created with.
base::UnkAllocator* fn_7100083990();

namespace base {

template <class T>
struct UnkVector {
    T* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;  // capacity grows to the next multiple of this
    bool field_0x14 = true;
    u32 flags = 0x3e;  // passed to the allocator
    UnkAllocator* allocator = fn_7100083990();

    // Inlined in fn_7100a19530. A capacity that doesn't grow past the count (no step) hangs
    // in an empty loop, as in the original. Elements are copied bitwise: the original reloads
    // `count` after storing the element, which a typed store wouldn't force.
    void PushBack(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        T* p;
        if (old_capacity < new_capacity) {
            p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(T));
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        __builtin_memcpy(&p[n], &value, sizeof(T));
        ++count;
    }
};
static_assert(sizeof(UnkVector<void*>) == 0x28, "UnkVector layout");

}  // namespace base

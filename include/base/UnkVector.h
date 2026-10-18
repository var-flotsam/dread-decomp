#pragma once

// The engine's growable array and the allocator interface it allocates through. Neither name
// is known; both keep placeholders. The layout is shared by CType+0x90 (default-constructed
// there: growth step 10, flags 0x3e) and the parameter-type list of a reflected function
// (UnkReflFunction+0x8, filled by fn_7100a19530).

#include <new>

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

    UnkVector() = default;
    explicit UnkVector(u32 flags) : flags(flags) {}

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

    // The element at `index`. When field_0x14 is set, an index past the end first grows the
    // array to the next multiple of the step and then to exactly index + 1 elements,
    // value-initialising the new ones (fn_71000305d8, fn_71002b9ed8 and their copies). The
    // capacity stays in a register between the two steps.
    T& GrowAt(u32 index) {
        if (field_0x14 && count <= index) {
            u32 cap = capacity;
            if (cap <= index && step != 0) {
                u32 grown = (index / step + 1) * step;
                if (cap < grown) {
                    Reallocate(grown);
                    cap = grown;
                }
            }
            u32 n = index + 1;
            if (cap < n)
                Reallocate(n);
            if constexpr (kTrivial) {
                __builtin_memset(&data[count], 0, (u64)(n - count) * sizeof(T));
            } else {
                for (u32 i = count; i < n; ++i)
                    new (&data[i]) T();
            }
            count = n;
        }
        return data[index];
    }

    // Moves `value` in at `index`, shifting the elements after it up (fn_71000d37d8,
    // fn_71002b9c74 and their copies). Appending grows like PushBack, including its hang.
    void Insert(T&& value, u32 index) {
        u32 cap = capacity;
        u32 n = count;
        T* p;
        if (n == index) {
            u32 new_capacity = cap;
            if (cap <= n && step != 0)
                new_capacity = (index / step + 1) * step;
            if ((s32)new_capacity <= (s32)index)
                for (;;) {
                }
            if (cap < new_capacity) {
                p = MoveTo(new_capacity);
                index = count;
                data = p;
                capacity = new_capacity;
            } else {
                p = data;
            }
        } else {
            if (cap <= n && step != 0) {
                u32 grown = (n / step + 1) * step;
                if (cap < grown) {
                    Reallocate(grown);
                    n = count;
                }
            }
            for (u32 i = n - 1; i >= index; --i) {
                T* q = &data[i];
                new (q + 1) T(static_cast<T&&>(*q));
                data[i].~T();
                if (i == 0)
                    break;
            }
            p = data;
        }
        new (&p[index]) T(static_cast<T&&>(value));
        ++count;
    }

    // PushBack's growth with a move-constructed element.
    void EmplaceBack(T&& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        if (old_capacity < new_capacity) {
            Reallocate(new_capacity);
            n = count;
        }
        new (&data[n]) T(static_cast<T&&>(value));
        ++count;
    }

private:
    static constexpr bool kTrivial = __is_trivially_copyable(T);

    // Trivially copyable elements are copied with memcpy; others are moved one by one and the
    // old ones destroyed.
    // Allocates new_capacity elements and moves the elements over, freeing the old array.
    T* MoveTo(u32 new_capacity) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
        if (data) {
            if constexpr (kTrivial) {
                __builtin_memcpy(p, data, (u64)count * sizeof(T));
            } else {
                T* src = data;
                T* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst) {
                    new (dst) T(static_cast<T&&>(*src));
                    src->~T();
                }
            }
            allocator->vf_0x48(data);
        }
        return p;
    }

    T* Reallocate(u32 new_capacity) {
        T* p = MoveTo(new_capacity);
        data = p;
        capacity = new_capacity;
        return p;
    }

public:
};
static_assert(sizeof(UnkVector<void*>) == 0x28, "UnkVector layout");

}  // namespace base

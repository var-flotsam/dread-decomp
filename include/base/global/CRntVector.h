#pragma once

// The engine's growable array, base::global::CRntVector (the name reflection gives it), and the
// allocator interface it allocates through (base::IAllocator, an inferred name: no RTTI, string or
// mercury-engine-data-structures entry names it). The layout is shared by CType+0x90 (default-constructed
// there: growth step 10, flags 0x3e) and the parameter-type list of a reflected function
// (CFunction+0x8, filled by fn_7100a19530).

#include <new>

#include "types.h"

// A translation unit that has the element accessor inlined, rather than calling the out-of-line
// copy, defines this as `__attribute__((always_inline))` before including the header.
#ifndef UNK_VECTOR_GROW_AT_ATTR
#define UNK_VECTOR_GROW_AT_ATTR
#endif

namespace base {

// Only the slots the array and the C library functions use are named. vf_0x40 allocates (size,
// alignment, flags); vf_0x48 frees; vf_0x50 is an allocation's usable size. An interface: its users
// reach it only through these slots, and two implementations share them (base::UnkDefaultAllocator,
// which GetDefaultAllocator hands out, and its sub-allocators; base/UnkDefaultAllocator.h).
struct IAllocator {
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
    virtual u64 vf_0x50(void* ptr);
};

}  // namespace base

// Creates that allocator over [mem, mem + size) the first time it is called (nninitStartup gives
// it the memory heap's first 0x4e700000 bytes; src/base/UnkDefaultAllocatorGlobal.cpp).
void fn_7100083870(void* mem, u64 size);
// Returns the allocator new arrays are created with.
base::IAllocator* GetDefaultAllocator();
// Allocates `size` bytes from that allocator (its vf_0x40, 8-byte aligned) with the given flags.
void* fn_71000839a0(u64 size, u32 flags);
// Allocates `size` bytes from that allocator, 8-byte aligned, with the given flags (the system
// start-up fn_71001325d0 takes the RomFS cache with flags 0x16; src/base/UnkDefaultAllocatorGlobal.cpp).
void* fn_7100083ba0(u64 size, u32 flags);
// The game's own operator new, new[], delete and delete[] (0x7100083b60, 0x7100083b80,
// 0x7100083c00, 0x7100083c30) and the allocation from a given allocator next to them: all through
// the default allocator (flags 2). Named by address, because on the port the C++ names are the
// runtime's (port/runtime/src/heap.cpp, which comes here once the allocator exists).
void* fn_7100083b60(u64 size);
void* fn_7100083b80(u64 size);
void* fn_7100083bc0(u64 size, u32 flags, u64 align, base::IAllocator* allocator);
void fn_7100083c00(void* ptr);
void fn_7100083c30(void* ptr);

namespace base::global {

template <class T>
struct CRntVector {
    using value_type = T;

    T* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;  // capacity grows to the next multiple of this
    bool field_0x14 = true;
    u32 flags = 0x3e;  // passed to the allocator
    IAllocator* allocator = GetDefaultAllocator();

    CRntVector() = default;
    explicit CRntVector(u32 flags) : flags(flags) {}
    // With the allocation flags and the growth step.
    CRntVector(u32 flags, u32 step) : step(step), flags(flags) {}

    // Takes the source's settings and allocator, reserves its capacity and appends copies of its
    // elements (fn_710001e3b4 for base::core::AssetName; fn_71000cbae4 inlines all of it).
    CRntVector(const CRntVector& other);

    // Takes over other's array, leaving it empty (fn_710001e28c for base::core::AssetName).
    CRntVector(CRntVector&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    CRntVector& operator=(const CRntVector& other) {
        if (this != &other)
            Assign(other);
        return *this;
    }
    // Copies other's elements over this one's (fn_710001e744).
    void Assign(const CRntVector& other);

    // Equal when the counts are and every element compares equal, the other's element on the left
    // (fn_710001e1d0 for base::core::AssetName, and the pointer and number vectors' Equals).
    bool operator==(const CRntVector& other) const {
        if (count != other.count)
            return false;
        for (u32 i = 0; i < count; ++i)
            if (!(other.data[i] == data[i]))
                return false;
        return true;
    }

    // Destroys the elements from `n` on and keeps the first n (fn_710001e62c).
    void Truncate(u32 n) {
        for (u32 i = n; i < count; ++i)
            data[i].~T();
        count = n;
    }

    // Copies `value` in at `index`, shifting the elements after it up (fn_710001e870).
    void InsertCopy(const T& value, u32 index) {
        u32 n = count;
        u32 cap = capacity;
        if (n == index) {
            if (cap <= n && step != 0)
                cap = (index / step + 1) * step;
            if ((s32)cap <= (s32)index)
                for (;;) {
                }
            Reserve(cap);
            new (&data[count]) T(value);
        } else {
            if (cap <= n && step != 0)
                cap = (n / step + 1) * step;
            Reserve(cap);
            for (u32 i = count - 1; i >= index; --i) {
                new (&data[i + 1]) T(static_cast<T&&>(data[i]));
                data[i].~T();
                if (i == 0)
                    break;
            }
            new (&data[index]) T(value);
        }
        ++count;
    }

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

    // Destroys the elements and frees the array (fn_71002b9b2c and its copies).
    void Free() {
        if (data) {
            Clear();
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    // Destroys the elements, keeping the array.
    void Clear() { Truncate(0); }

    // The element at `index`, without growing (GrowAt grows).
    T& operator[](u32 index) { return data[index]; }
    const T& operator[](u32 index) const { return data[index]; }

    T* begin() { return data; }
    T* end() { return data + count; }
    const T* begin() const { return data; }
    const T* end() const { return data + count; }

    // The capacity a full array grows to when it holds `n` elements: the next multiple of the step
    // above n (the growth of PushBack, EmplaceBack and Insert).
    u32 GrownCapacity(u32 n) const { return (n / step + 1) * step; }

    // Grows the array to exactly `n` elements, value-initialising the new ones; a smaller `n` only
    // lowers the count, destroying nothing (the bucket array of the map constructor in
    // src/game/CPowerBombMovement/CPowerBombMovement_Map_ovb0015.cpp).
    void Resize(u32 n) {
        u32 old = count;
        if (old < n) {
            T* p;
            if (capacity >= n) {
                p = data;
            } else {
                p = MoveTo(n);
                capacity = n;
                old = count;
                data = p;
            }
            if constexpr (kTrivial) {
                __builtin_memset(p + old, 0, (u64)(n - old) * sizeof(T));
            } else {
                for (u32 i = old; i < n; ++i)
                    new (&p[i]) T();
            }
        } else if (old == n) {
            return;
        }
        count = n;
    }

    // Takes over other's array after destroying the elements (the old array is not freed;
    // fn_71002b9bbc and its copies).
    CRntVector& operator=(CRntVector&& other) {
        if (this != &other) {
            Clear();
            allocator = other.allocator;
            flags = other.flags;
            step = other.step;
            field_0x14 = other.field_0x14;
            count = other.count;
            capacity = other.capacity;
            data = other.data;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // Destroys the element at `index` and moves the later ones down. Unverified: fn_71002b9908
    // keeps the loop counter and the source offset in separate registers.
    void EraseAt(u32 index) {
        if (index < count) {
            if constexpr (kTrivial) {
                T* p = data + index;
                __builtin_memcpy(p, p + 1, (count - index - 1) * sizeof(T));
            } else {
                data[index].~T();
                for (u32 i = index; i < count - 1; ++i) {
                    new (&data[i]) T(static_cast<T&&>(data[i + 1]));
                    data[i + 1].~T();
                }
            }
            count = count - 1;
        }
    }

    // Removes the first element equal to `value`, shifting the rest down bitwise
    // (fn_71002ae87c inlines it).
    void Remove(const T& value) {
        u32 n = count;
        for (u32 i = 0; i < n; ++i) {
            if (data[i] == value) {
                __builtin_memcpy(&data[i], &data[i + 1], (u64)(n - i - 1) * sizeof(T));
                --count;
                return;
            }
        }
    }

    // The element at `index`. When field_0x14 is set, an index past the end first grows the
    // array to the next multiple of the step and then to exactly index + 1 elements,
    // value-initialising the new ones (fn_71000305d8, fn_71002b9ed8 and their copies). The
    // capacity stays in a register between the two steps.
    UNK_VECTOR_GROW_AT_ATTR T& GrowAt(u32 index) {
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

    // EmplaceBack with a copy-constructed element.
    void PushBackCopy(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        n = count;
        new (&data[n]) T(value);
        ++count;
    }

    // Grows the array to at least n elements, moving the elements over and freeing the old array
    // (fn_710001e4e8 for base::core::AssetName; fn_7100c77cd0 inlines it).
    void Reserve(u32 n) {
        if (capacity < n) {
            T* p = static_cast<T*>(allocator->vf_0x40((u64)n * sizeof(T), 8, flags));
            if (data) {
                if constexpr (kTrivial) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(T));
                } else {
                    T* src = data;
                    T* dst = p;
                    for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                        new (dst) T(static_cast<T&&>(*src));
                        src->~T();
                    }
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }

    // InsertCopy with a moved element (fn_710001ead8). Trial name: Insert above grows inline.
    void InsertMove(T&& value, u32 index) {
        u32 n = count;
        u32 cap = capacity;
        if (n == index) {
            if (cap <= n && step != 0)
                cap = (index / step + 1) * step;
            if ((s32)cap <= (s32)index)
                for (;;) {
                }
            Reserve(cap);
            new (&data[count]) T(static_cast<T&&>(value));
        } else {
            if (cap <= n && step != 0)
                cap = (n / step + 1) * step;
            Reserve(cap);
            for (u32 i = count - 1; i >= index; --i) {
                new (&data[i + 1]) T(static_cast<T&&>(data[i]));
                data[i].~T();
                if (i == 0)
                    break;
            }
            new (&data[index]) T(static_cast<T&&>(value));
        }
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
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
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
static_assert(sizeof(CRntVector<void*>) == 0x28, "CRntVector layout");

template <class T>
CRntVector<T>::CRntVector(const CRntVector& other)
    : step(other.step), field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
    Reserve(other.capacity);
    if constexpr (kTrivial) {
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(T));
        count = other.count;
    } else if (other.count != 0) {
        const T* end = other.data + other.count;
        for (const T* p = other.data; p != end; ++p) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve(new_capacity);
            new (&data[count]) T(*p);
            ++count;
        }
    }
}


template <class T>
void CRntVector<T>::Assign(const CRntVector& other) {
    Clear();
    Reserve(other.capacity);
    if constexpr (kTrivial) {
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(T));
        count = other.count;
    } else if (other.count != 0) {
        const T* end = other.data + other.count;
        for (const T* p = other.data; p != end; ++p) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve(new_capacity);
            new (&data[count]) T(*p);
            ++count;
        }
    }
}

}  // namespace base::global

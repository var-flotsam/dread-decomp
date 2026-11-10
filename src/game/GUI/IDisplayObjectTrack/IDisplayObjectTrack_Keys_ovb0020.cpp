// GUI::IDisplayObjectTrack unit (tools/tu_map.py unit 1671): arrays (base::UnkVector's layout) of track keys, a
// time and a float (fn_7100b172f8, fn_7100b175ec) or a flag (fn_7100b18bfc, fn_7100b18dc8): the growing element
// accessor (an index past the end grows the array to the next multiple of the step, then to exactly index + 1,
// filling it with zeroed keys) and the copy constructor (the capacity first, then each key appended). Elements are
// copied bitwise, one by one. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

// A key of a float track: a time and a value.
struct UnkFloatKey_ovb0020 {
    s32 time;
    f32 value;

    UnkFloatKey_ovb0020() : time(0), value(0.0f) {}
};

// A key of a flag track: a time and a flag.
struct UnkFlagKey_ovb0020 {
    s32 time;
    bool value;

    UnkFlagKey_ovb0020() : time(0), value(false) {}
};

// base::UnkVector's layout, with its own growing element accessor.
template <class T>
struct UnkKeyArray_ovb0020 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grow;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 n) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)n * sizeof(T), 8, flags));
        if (T* old = data) {
            T* dst = p;
            T* src = old;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                __builtin_memcpy(dst, src, sizeof(T));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    __attribute__((always_inline)) UnkKeyArray_ovb0020(const UnkKeyArray_ovb0020& other)
        : data(nullptr), count(0), capacity(0) {
        step = other.step;
        grow = other.grow;
        flags = other.flags;
        allocator = other.allocator;
        if (u32 n = other.capacity)
            Reallocate(n);
        for (const T *it = other.data, *end = other.data + other.count; it != end; ++it)
            PushBack(*it);
    }

    __attribute__((always_inline)) void PushBack(const T& value) {
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
        __builtin_memcpy(&data[n], &value, sizeof(T));
        ++count;
    }

    __attribute__((always_inline)) T& At(u32 index) {
        if (grow && count <= index) {
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
            for (u32 i = count; i < n; ++i)
                data[i] = T();
            count = n;
        }
        return data[index];
    }
};

// Declared as src/flag_id/fn_7100b17e30.cpp does.
u64* fn_7100b172f8(void* self, u32 key) {
    auto* keys = static_cast<UnkKeyArray_ovb0020<UnkFloatKey_ovb0020>*>(self);
    return reinterpret_cast<u64*>(&keys->At(key));
}

UnkFlagKey_ovb0020* fn_7100b18bfc(UnkKeyArray_ovb0020<UnkFlagKey_ovb0020>* keys, u32 index) {
    return &keys->At(index);
}

void fn_7100b175ec(UnkKeyArray_ovb0020<UnkFloatKey_ovb0020>* self, const UnkKeyArray_ovb0020<UnkFloatKey_ovb0020>* other) {
    new (self) UnkKeyArray_ovb0020<UnkFloatKey_ovb0020>(*other);
}

void fn_7100b18dc8(UnkKeyArray_ovb0020<UnkFlagKey_ovb0020>* self, const UnkKeyArray_ovb0020<UnkFlagKey_ovb0020>* other) {
    new (self) UnkKeyArray_ovb0020<UnkFlagKey_ovb0020>(*other);
}

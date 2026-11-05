// Unit 83: the "insert at index" function (base::UnkVector::Insert) of arrays of reflected values
// (0x18 bytes: moved with the copy constructor fn_71000a179c and destroyed with fn_71000a1c60, as in
// src/base/reflection/UnkReflValueVectorCopy.cpp; fn_71000d5c20) and of strings (fn_71000e0988 moves
// the new one in, fn_71000e0758 copies it). Names are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

__attribute__((flatten)) void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

struct UnkReflValueElem_b62a1 {
    UnkReflValue value;

    UnkReflValueElem_b62a1(UnkReflValueElem_b62a1&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflValueElem_b62a1() { fn_71000a1c60(&value); }
};

__attribute__((flatten)) void fn_71000d5c20(base::UnkVector<UnkReflValueElem_b62a1>* self, UnkReflValueElem_b62a1* value, u32 index) {
    self->Insert(static_cast<UnkReflValueElem_b62a1&&>(*value), index);
}
// Strings (base::global::CRntString, 0x30 bytes): moved with fn_710007eab4, copied with
// fn_710007ea60 (declared for UnkNamedEntry, the same layout, which the element holds), destroyed with fn_710007ecd4.
struct UnkStringElem_b62a1 {
    base::UnkNamedEntry value;

    base::global::CRntString* Str() { return reinterpret_cast<base::global::CRntString*>(&value); }

    UnkStringElem_b62a1(UnkStringElem_b62a1&& other) { fn_710007eab4(Str(), other.Str()); }
    UnkStringElem_b62a1(const UnkStringElem_b62a1& other) { fn_710007ea60(&value, &other.value); }
    ~UnkStringElem_b62a1() { fn_710007ecd4(Str()); }
};

// base::UnkVector::Insert with the new element copied in (the header's moves it).
template <class T>
__attribute__((always_inline)) static inline void InsertCopy_b62a1(base::UnkVector<T>* self, const T& value,
                                                                   u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    T* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = static_cast<T*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, self->flags));
            if (self->data) {
                T* src = self->data;
                T* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) T(static_cast<T&&>(*src));
                    src->~T();
                }
                self->allocator->vf_0x48(self->data);
            }
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                T* q = static_cast<T*>(self->allocator->vf_0x40((u64)grown * sizeof(T), 8, self->flags));
                if (self->data) {
                    T* src = self->data;
                    T* dst = q;
                    for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                        new (dst) T(static_cast<T&&>(*src));
                        src->~T();
                    }
                    self->allocator->vf_0x48(self->data);
                }
                self->data = q;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            T* q = &self->data[i];
            new (q + 1) T(static_cast<T&&>(*q));
            self->data[i].~T();
            if (i == 0)
                break;
        }
        p = self->data;
    }
    new (&p[index]) T(value);
    ++self->count;
}

void fn_71000e0758(base::UnkVector<UnkStringElem_b62a1>* self, const UnkStringElem_b62a1* value, u32 index) {
    InsertCopy_b62a1(self, *value, index);
}
__attribute__((flatten)) void fn_71000e0988(base::UnkVector<UnkStringElem_b62a1>* self, UnkStringElem_b62a1* value,
                                            u32 index) {
    self->Insert(static_cast<UnkStringElem_b62a1&&>(*value), index);
}

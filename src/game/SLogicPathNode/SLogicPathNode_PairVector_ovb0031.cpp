// SLogicPathNode unit: an array (base::UnkVector, include/base/UnkVector.h) of 8-byte two-word
// elements whose moves copy each word bitwise (the original reloads the count and the array after
// each element store). fn_71010605cc copy-constructs such an array (the other's capacity reserved,
// then each element appended); fn_710106099c and fn_7101060bb4 move an element in at `index`,
// shifting the later ones up. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// An 8-byte element of two words, copied member by member.
struct UnkPathPair {
    u32 value[2];

    // Each word is copied bitwise.
    void CopyFrom(const UnkPathPair& other) {
        __builtin_memcpy(&value[0], &other.value[0], sizeof(u32));
        __builtin_memcpy(&value[1], &other.value[1], sizeof(u32));
    }
    UnkPathPair(const UnkPathPair& other) { CopyFrom(other); }
    UnkPathPair(UnkPathPair&& other) { CopyFrom(other); }
};

// Allocates new_capacity elements and moves the elements over, freeing the old array.
template <class T>
__attribute__((always_inline)) static inline T* MoveTo(base::UnkVector<T>* self, u32 new_capacity) {
    T* p = static_cast<T*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, self->flags));
    if (self->data) {
        T* src = self->data;
        T* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) T(static_cast<T&&>(*src));
            src->~T();
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

__attribute__((flatten)) void fn_71010605cc(base::UnkVector<UnkPathPair>* self, const base::UnkVector<UnkPathPair>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkPathPair *it = other->data, *end = it + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}

// base::UnkVector::Insert with the new element's slot taken in each branch.
template <class T>
__attribute__((always_inline)) static inline void InsertAt(base::UnkVector<T>* self, T* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        T* p;
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) T(static_cast<T&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                T* p = MoveTo(self, grown);
                n = self->count;
                self->data = p;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            T* q = &self->data[i];
            new (q + 1) T(static_cast<T&&>(*q));
            self->data[i].~T();
            if (i == 0)
                break;
        }
        new (&self->data[index]) T(static_cast<T&&>(*value));
    }
    ++self->count;
}

void fn_710106099c(base::UnkVector<UnkPathPair>* self, UnkPathPair* value, u32 index) {
    InsertAt(self, value, index);
}

void fn_7101060bb4(base::UnkVector<UnkPathPair>* self, UnkPathPair* value, u32 index) {
    InsertAt(self, value, index);
}

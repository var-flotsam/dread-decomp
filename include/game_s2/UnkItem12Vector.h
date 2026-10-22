#pragma once

// An array of 12-byte elements (moved with a memcpy of the whole element) and the Insert that
// fn_71003ee274 and fn_71003ee494 share. It is UnkVector<T>::Insert written out: the append
// path and the shifting path each compute the slot themselves (the append one with byte
// arithmetic, which keeps the compiler from merging the two stores).

#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkItem12 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkItem12() = default;
    UnkItem12(UnkItem12&& other) { __builtin_memcpy(this, &other, sizeof(UnkItem12)); }
    ~UnkItem12() {}
};

__attribute__((always_inline)) static inline void InsertItem12(base::UnkVector<UnkItem12>* self, UnkItem12* value,
                                                              u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    UnkItem12* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = static_cast<UnkItem12*>(
                self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkItem12), 8, self->flags));
            if (UnkItem12* src = self->data) {
                UnkItem12* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) UnkItem12(static_cast<UnkItem12&&>(*src));
                    src->~UnkItem12();
                }
                self->allocator->vf_0x48(self->data);
            }
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (reinterpret_cast<char*>(p) + (u64)index * sizeof(UnkItem12)) UnkItem12(static_cast<UnkItem12&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkItem12* q =
                    static_cast<UnkItem12*>(self->allocator->vf_0x40((u64)grown * sizeof(UnkItem12), 8, self->flags));
                if (UnkItem12* src = self->data) {
                    UnkItem12* dst = q;
                    for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                        new (dst) UnkItem12(static_cast<UnkItem12&&>(*src));
                        src->~UnkItem12();
                    }
                    self->allocator->vf_0x48(self->data);
                }
                self->data = q;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkItem12* q = &self->data[i];
            new (q + 1) UnkItem12(static_cast<UnkItem12&&>(*q));
            self->data[i].~UnkItem12();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkItem12(static_cast<UnkItem12&&>(*value));
    }
    ++self->count;
}

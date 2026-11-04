// Unit 83: shrinks keyed arrays (values and their string-id keys side by side) to their count, in
// the shape of src/game/CMinimapDef_ShrinkToFit_b49.cpp: the values are moved over and the
// moved-from ones released, the keys copied and released, and the old arrays freed. Here the
// values are string ids (fn_71000d9c10) or file-path ids (fn_71000db654); fn_71000d97fc and
// fn_71000db238 are the same arrays' reserve. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A string id over `Pool` taken over on a move (the moved-from id left empty through an untyped
// store: the original reloads the count after it), released in two steps (as CStrId::Reset).
template <base::global::UnkStringPool*& Pool>
struct UnkMovedId_b62a1 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMovedId_b62a1(UnkMovedId_b62a1&& other) : UnkStrIdBase(other) {
        base::global::UnkStringData* empty = &Pool->empty;
        __builtin_memcpy(&other.data, &empty, sizeof(empty));
    }
    ~UnkMovedId_b62a1() {
        if (base::global::UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkMovedStrId_b62a1 = UnkMovedId_b62a1<g_unk_7101d47108>;
using UnkMovedPathId_b62a1 = UnkMovedId_b62a1<g_unk_7101d47200>;

template <class T>
struct UnkKeyedArray_b62a1 {
    T* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Moves the values and keys into new arrays of `capacity` entries (the keys are moved too, unlike
// the shrink's).
template <class T>
__attribute__((always_inline)) static inline void MoveTo_b62a1(UnkKeyedArray_b62a1<T>* self, u32 capacity) {
    T* values = static_cast<T*>(self->allocator->vf_0x40((u64)capacity * sizeof(T), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (T* src = self->values) {
        T* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) T(static_cast<T&&>(*src));
            src->~T();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            auto* from = reinterpret_cast<UnkMovedStrId_b62a1*>(old);
            new (&keys[j]) UnkMovedStrId_b62a1(static_cast<UnkMovedStrId_b62a1&&>(*from));
            from->~UnkMovedStrId_b62a1();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}

template <class T>
__attribute__((always_inline)) static inline void Reserve_b62a1(UnkKeyedArray_b62a1<T>* self, u32 capacity) {
    if (self->capacity < capacity)
        MoveTo_b62a1(self, capacity);
}

template <class T>
__attribute__((always_inline)) static inline void ShrinkToFit_b62a1(UnkKeyedArray_b62a1<T>* self) {
    if (self->count == self->capacity)
        return;
    T* values = static_cast<T*>(self->allocator->vf_0x40((u64)self->count * sizeof(T), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (T* src = self->values) {
        T* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) T(static_cast<T&&>(*src));
            src->~T();
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) CStrId(*old);
            old->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = self->count;
}

void fn_71000d97fc(UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>* self, u32 capacity) { Reserve_b62a1(self, capacity); }
void fn_71000d9c10(UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>* self) { ShrinkToFit_b62a1(self); }
void fn_71000db238(UnkKeyedArray_b62a1<UnkMovedPathId_b62a1>* self, u32 capacity) { Reserve_b62a1(self, capacity); }
void fn_71000db654(UnkKeyedArray_b62a1<UnkMovedPathId_b62a1>* self) { ShrinkToFit_b62a1(self); }

// The reflected End (and an Erase) of arrays whose elements' destructor is out of line
// (0x80-byte elements destroyed by fn_7100ad04c4 and moved by fn_7100acf988, 0x40-byte ones
// destroyed by fn_7100c8a490, 0xa0-byte ones by fn_7100c886c0), and of an array of words.
#include "base/UnkCollectionIter.h"
#include "types.h"

template <u32 Size, void (*Destroy)(void*), void (*Move)(void*, void*) = nullptr>
struct UnkOutOfLineElement {
    u64 unk_0x0[Size / 8];

    UnkOutOfLineElement(UnkOutOfLineElement&& other) { Move(this, &other); }
    ~UnkOutOfLineElement() { Destroy(this); }
};

void fn_7100ad04c4(void* self);
void fn_7100acf988(void* self, void* other);
void fn_7100c8a490(void* self);
using UnkElement80 = UnkOutOfLineElement<0x80, fn_7100ad04c4, fn_7100acf988>;
using UnkElement40 = UnkOutOfLineElement<0x40, fn_7100c8a490>;
void fn_7100c886c0(void* self);
using UnkElementA0 = UnkOutOfLineElement<0xa0, fn_7100c886c0>;

using UnkElement80Iter = base::UnkCollectionIter<UnkElement80>;
using UnkElement40Iter = base::UnkCollectionIter<UnkElement40>;
using UnkElementA0Iter = base::UnkCollectionIter<UnkElementA0>;

void fn_7100ad2800(const base::UnkVector<UnkElement80>* v, UnkElement80Iter* out) {
    *out = UnkElement80Iter(v->count, v->data + v->count);
}
void fn_7100c92a28(const base::UnkVector<UnkElement40>* v, UnkElement40Iter* out) {
    *out = UnkElement40Iter(v->count, v->data + v->count);
}
void fn_7100c8f61c(const base::UnkVector<UnkElementA0>* v, UnkElementA0Iter* out) {
    *out = UnkElementA0Iter(v->count, v->data + v->count);
}

// Erase: destroys the element at the iterator's index and moves the later ones down.
template <class T>
__attribute__((always_inline)) static inline void Erase(base::UnkVector<T>* v, const base::UnkCollectionIter<T>* it) {
    u32 index = it->index;
    if (index < v->count) {
        u64 i = index;
        v->data[i].~T();
        for (; i < (u32)(v->count - 1); ++i) {
            new (&v->data[i]) T(static_cast<T&&>(v->data[i + 1]));
            v->data[i + 1].~T();
        }
        v->count = v->count - 1;
    }
}

// NON_MATCHING(minor): the original keeps the index and a 57-bit masked copy of it (as from an
// element offset divided by the size) in separate registers.
void fn_7100ad1d40(base::UnkVector<UnkElement80>* v, const UnkElement80Iter* it) { Erase(v, it); }

// The End of an array of words.
using UnkWordIter = base::UnkCollectionIter<u32>;

void fn_7100adbb44(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }
void fn_7100eb2c10(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }

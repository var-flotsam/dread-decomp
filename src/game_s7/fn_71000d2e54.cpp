// Copies of fn_7100adbb44 (src/game_aca57c/fn_7100ad2800.cpp), which the binary
// repeats at other addresses: fn_71000d2e54, fn_71000d4c68, fn_7100a35b54, fn_7100a36a34.
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

// The End of an array of words.
using UnkWordIter = base::UnkCollectionIter<u32>;

void fn_71000d2e54(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }

void fn_71000d4c68(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }

void fn_7100a35b54(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }

void fn_7100a36a34(const base::UnkVector<u32>* v, UnkWordIter* out) { *out = UnkWordIter(v->count, v->data + v->count); }


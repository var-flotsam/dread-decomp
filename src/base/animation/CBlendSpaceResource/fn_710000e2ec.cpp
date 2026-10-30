// Copy of fn_7100a954c8 (src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp) for another element type, naming its own callees and data (the
// comments are adapted from the original's): fn_710000e2ec. Copy-construction of an array of 0x18-byte elements: the
// settings are taken over, the array reserves the other's capacity (out of line, fn_710000e610) and appends a copy of
// each element (fn_710000e7a4).
#include "base/UnkFreedVectorPair.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem18 {
    u64 unk_0x0[3];
};

struct UnkElem18Vector;
void fn_710000e610(UnkElem18Vector* v, u32 n);
void fn_710000e7a4(UnkElem18Vector* v, const UnkElem18* value);

struct UnkElem18Vector {
    UnkElem18* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

};

// The copy itself, for any array in this layout with an out-of-line Reserve and push-back.
template <class Array, class Elem, void (*Reserve)(Array*, u32), void (*PushBack)(Array*, const Elem*)>
__attribute__((always_inline)) static inline void CopyConstruct(Array* self, const Array& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    Reserve(self, other.capacity);
    for (const Elem *p = other.data, *end = p + other.count; p != end; ++p)
        PushBack(self, p);
}

void fn_710000e2ec(UnkElem18Vector* self, const UnkElem18Vector* other) {
    CopyConstruct<UnkElem18Vector, UnkElem18, fn_710000e610, fn_710000e7a4>(self, *other);
}

// The same for an array of 0x60-byte records (base/UnkFreedVectorPair.h) in another unit, through its
// own reserve and push-back.
void fn_7100e52898(UnkPairArray* self, const UnkPairArray* other) {
    CopyConstruct<UnkPairArray, UnkFreedVectorPair, fn_7100e52b58, fn_7100e52cf4>(self, *other);
}

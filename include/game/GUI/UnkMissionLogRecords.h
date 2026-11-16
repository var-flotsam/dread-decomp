#pragma once

// The mission log's record arrays (CMissionLog's unit): growable arrays of 0x38-byte records, with the members
// base::global::CRntVector has but a 32-bit count. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkElem38 {
    u8 unk_0x0[0x38];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

using UnkElem38Array = UnkElemArray<UnkElem38>;

// Reserves room for `capacity` records.
void fn_7100b5c28c(UnkElem38Array* self, u32 capacity);
// Appends a copy of `value`.
void fn_7100b593d0(UnkElem38Array* self, const UnkElem38* value);
// Destroys the records and frees the array (the destructor body).
void fn_7100b55d4c(UnkElem38Array* self);

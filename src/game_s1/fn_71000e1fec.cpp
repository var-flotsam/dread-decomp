// Frees a vector held at an offset inside its owner: the vector's own Free (as in fn_71000e01c0,
// src/reflection_types/collection_types_00.cpp) inlined into the owner. Strings (CRntString)
// at +0x8 (fn_71000e1fec); 0x50-byte records starting with a string at +0x10 (fn_710026ead0).
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

template <class T, u64 Offset>
struct UnkVectorAt {
    u8 unk_0x0[Offset];
    base::UnkVector<T> items;
};

struct UnkStringRecord_0x50 {
    base::global::CRntString name;
    u8 unk_0x30[0x20];
};

void fn_71000e1fec(UnkVectorAt<base::global::CRntString, 0x8>* self) { self->items.Free(); }

void fn_710026ead0(UnkVectorAt<UnkStringRecord_0x50, 0x10>* self) { self->items.Free(); }

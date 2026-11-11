// Frees a vector held at an offset inside its owner: the vector's own Free (as in fn_71000e01c0,
// src/reflection_types/collection_types_00.cpp) inlined into the owner. Strings (CRntString)
// at +0x8 (fn_71000e1fec); 0x50-byte tree nodes at +0x10, each with its own children at +0x10, so
// freeing a node's children frees theirs through the same function (fn_710026ead0 calls itself).
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

template <class T, u64 Offset>
struct UnkVectorAt {
    u8 unk_0x0[Offset];
    base::UnkVector<T> items;
};

struct UnkTreeNode_0x50;
void fn_710026ead0(UnkVectorAt<UnkTreeNode_0x50, 0x10>* self);

struct UnkTreeNode_0x50 {
    u8 unk_0x0[0x50];
    __attribute__((always_inline)) ~UnkTreeNode_0x50() {
        fn_710026ead0(reinterpret_cast<UnkVectorAt<UnkTreeNode_0x50, 0x10>*>(this));
    }
};

void fn_71000e1fec(UnkVectorAt<base::global::CRntString, 0x8>* self) { self->items.Free(); }

void fn_710026ead0(UnkVectorAt<UnkTreeNode_0x50, 0x10>* self) { self->items.Free(); }

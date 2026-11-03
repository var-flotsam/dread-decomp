// animtree::CAnimTreeDef's unit: the begin and end functions of a reflected collection of
// 0x10-byte entries (two string ids). The iterator's constructor and assignment re-point its
// cached value through fn_71002d8bac, kept out of line (as
// src/base/UnkUnit25/UnkIdPathIterBeginEnd_b49.cpp). Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkAnimTreeIdPair_b49a1 {
    base::UnkStrIdMember first;
    base::UnkStrIdMember second;
};

struct UnkAnimTreeIdPairArray_b49a1 {
    UnkAnimTreeIdPair_b49a1* data;
    u32 count;
};

struct UnkAnimTreeIdPairIter_b49a1;
void fn_71002d8bac(UnkAnimTreeIdPairIter_b49a1* it);  // re-points `current` at (index, element)

struct UnkAnimTreeIdPairIter_b49a1 {
    u32 index;
    UnkAnimTreeIdPair_b49a1* element;
    base::UnkIterValue<UnkAnimTreeIdPair_b49a1, u32> current;

    UnkAnimTreeIdPairIter_b49a1(u32 index, UnkAnimTreeIdPair_b49a1* element) : index(index), element(element) {
        fn_71002d8bac(this);
    }
    UnkAnimTreeIdPairIter_b49a1& operator=(const UnkAnimTreeIdPairIter_b49a1& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_71002d8bac(this);
        }
        return *this;
    }
};

void fn_71002dae2c(UnkAnimTreeIdPairArray_b49a1* self, UnkAnimTreeIdPairIter_b49a1* out) {
    *out = UnkAnimTreeIdPairIter_b49a1(0, self->data);
}

void fn_71002daf38(UnkAnimTreeIdPairArray_b49a1* self, UnkAnimTreeIdPairIter_b49a1* out) {
    *out = UnkAnimTreeIdPairIter_b49a1(self->count, self->data + self->count);
}

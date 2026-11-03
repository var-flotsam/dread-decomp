// From unit 25 of tools/tu_map.py (0x7100018a44-0x710001f9ac): the begin and end functions of a
// reflected collection type (slots 0x160 and 0x168 in src/reflection_types/collection_types_00.cpp)
// over an array of (string id, file-path id) pairs. The iterator's constructor and assignment
// re-point its cached value through fn_710001c340, kept out of line. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkStrIdPathPair.h"
#include "types.h"

struct UnkUnit25PairArray_b49 {
    UnkStrIdPathPair* data;
    u32 count;
};

struct UnkUnit25PairIter_b49;
void fn_710001c340(UnkUnit25PairIter_b49* it);  // re-points `current` at (index, element)

struct UnkUnit25PairIter_b49 {
    u32 index;
    UnkStrIdPathPair* element;
    base::UnkIterValue<UnkStrIdPathPair, u32> current;

    UnkUnit25PairIter_b49(u32 index, UnkStrIdPathPair* element) : index(index), element(element) {
        fn_710001c340(this);
    }
    UnkUnit25PairIter_b49& operator=(const UnkUnit25PairIter_b49& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_710001c340(this);
        }
        return *this;
    }
};

void fn_710001ecfc(UnkUnit25PairArray_b49* self, UnkUnit25PairIter_b49* out) {
    *out = UnkUnit25PairIter_b49(0, self->data);
}

void fn_710001ee0c(UnkUnit25PairArray_b49* self, UnkUnit25PairIter_b49* out) {
    *out = UnkUnit25PairIter_b49(self->count, self->data + self->count);
}

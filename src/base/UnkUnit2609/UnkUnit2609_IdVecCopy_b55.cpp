// Unit 2609 of tools/tu_map.py (base::reflection names): copy constructors of a record of a string
// id and a vector of string ids (base::UnkStrIdVec, include/base/UnkCloneRecords.h), two copies.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "types.h"

template <int Tag>
struct UnkIdVecRecord_b55 {
    base::global::CStrId id;
    base::UnkStrIdVec ids;
};

void fn_7100f9fcc8(UnkIdVecRecord_b55<0>* self, const UnkIdVecRecord_b55<0>* other) {
    new (self) UnkIdVecRecord_b55<0>(*other);
}
void fn_7100f9fdf8(UnkIdVecRecord_b55<1>* self, const UnkIdVecRecord_b55<1>* other) {
    new (self) UnkIdVecRecord_b55<1>(*other);
}

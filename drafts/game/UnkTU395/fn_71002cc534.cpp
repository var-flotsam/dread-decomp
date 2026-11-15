// Unit 395: Begin of a reflected array of 16-byte plain elements, as base::refl_Begin.
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"

struct UnkUnit395BlockTag_b49a1;
using UnkUnit395Block_b49a1 = UnkCloneBlock<UnkUnit395BlockTag_b49a1, 0x10>;

void fn_71002cc534(const base::global::CRntVector<UnkUnit395Block_b49a1>* v,
                   base::UnkCollectionIter<UnkUnit395Block_b49a1>* out) {
    base::refl_Begin(v, out);
}

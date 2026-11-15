// SLogicPathNode unit: the reflected Begin of an array of 0x20-byte records: an iterator at index 0.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkRecord_0x20 {
    u32 field_0x0[8];
};

void fn_710105e474(const base::global::CRntVector<UnkRecord_0x20>* v, base::UnkCollectionIter<UnkRecord_0x20>* out) {
    base::UnkCollectionIter<UnkRecord_0x20> begin(0, v->data);
    *out = begin;
}

// End iterator of a vector of 12-byte elements (as in fn_71001205d0, src/game_s1/fn_71000d1f3c.cpp).
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkCollectionIter;
using base::UnkVector;

struct UnkElem_0xc {
    u32 words[3];
};

void fn_71003ee938(const UnkVector<UnkElem_0xc>* v, UnkCollectionIter<UnkElem_0xc>* out) { base::refl_End(v, out); }

// The same for 24-byte elements, in two other units.
struct UnkElem_0x18 {
    u64 words[3];
};

void fn_71004b3040(const UnkVector<UnkElem_0x18>* v, UnkCollectionIter<UnkElem_0x18>* out) { base::refl_End(v, out); }
void fn_71004c0c54(const UnkVector<UnkElem_0x18>* v, UnkCollectionIter<UnkElem_0x18>* out) { base::refl_End(v, out); }

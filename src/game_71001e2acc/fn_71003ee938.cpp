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

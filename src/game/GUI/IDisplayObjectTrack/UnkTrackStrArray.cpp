// The reflected Begin and End of an engine array of 0x38-byte string records
// (base/UnkCollectionIter.h), as fn_7100ad2728 and fn_71000e0e8c (src/game_aca57c/fn_7100ad2728.cpp,
// src/game_s1/fn_71000d5888.cpp) for this unit's element type.
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

// A 0x38-byte element: a word, then a string destroyed out of line.
struct UnkTrackStrElement {
    u64 field_0x0;
    u64 field_0x8[0x30 / 8];

    ~UnkTrackStrElement() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(field_0x8)); }
};

using UnkTrackStrIter = base::UnkCollectionIter<UnkTrackStrElement>;

void fn_7100b1b7d0(const base::UnkVector<UnkTrackStrElement>* v, UnkTrackStrIter* out) {
    *out = UnkTrackStrIter(0, v->data);
}

void fn_7100b1b8a8(const base::UnkVector<UnkTrackStrElement>* v, UnkTrackStrIter* out) {
    *out = UnkTrackStrIter(v->count, v->data + v->count);
}

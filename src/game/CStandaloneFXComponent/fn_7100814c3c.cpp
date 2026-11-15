// Copies of fn_7100c92464 (src/game_aca57c/fn_7100ad2004.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100814c3c.
// Move-assignments of engine arrays (base/global/CRntVector.h): the elements are destroyed, then the
// other array's storage and settings are taken over. One element type per copy.
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CRntString;

struct UnkElem80 {
    u8 unk_0x0[0x80];
    ~UnkElem80();  // fn_7100ad04c4
};

struct UnkNamedElem38 {
    u64 field_0x0;
    CRntString name;
    u8 unk_end[0x38 - 0x8 - sizeof(CRntString)];
};

struct UnkElemA0 {
    u8 unk_0x0[0xa0];
    ~UnkElemA0();  // fn_7100c886c0
};

struct UnkElem40_710081348c {
    u8 unk_0x0[0x40];
    ~UnkElem40_710081348c();  // fn_710081348c
};

struct UnkElem138 {
    u8 unk_0x0[0x138];
    ~UnkElem138();  // fn_7100e4e310
};

CRntVector<UnkElem40_710081348c>& fn_7100814c3c(CRntVector<UnkElem40_710081348c>* self,
                                               CRntVector<UnkElem40_710081348c>&& other) {
    return *self = static_cast<CRntVector<UnkElem40_710081348c>&&>(other);
}


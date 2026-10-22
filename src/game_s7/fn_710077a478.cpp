// Copies of fn_7100ad2004 (src/game_aca57c/fn_7100ad2004.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_710077a478.
// Move-assignments of engine arrays (base/UnkVector.h): the elements are destroyed, then the
// other array's storage and settings are taken over. One element type per copy.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::UnkVector;
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

struct UnkElem40 {
    u8 unk_0x0[0x40];
    ~UnkElem40();  // fn_7100c8a490
};

struct UnkElem138 {
    u8 unk_0x0[0x138];
    ~UnkElem138();  // fn_7100e4e310
};

UnkVector<UnkElem80>& fn_710077a478(UnkVector<UnkElem80>* self, UnkVector<UnkElem80>&& other) {
    return *self = static_cast<UnkVector<UnkElem80>&&>(other);
}


// Copies of fn_7100ad2004 (src/game_aca57c/fn_7100ad2004.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_710077a478.
// Move-assignments of engine arrays (base/global/CRntVector.h): the elements are destroyed, then the
// other array's storage and settings are taken over. One element type per copy.
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "game/UnkElem138.h"
#include "game/UnkElem40.h"
#include "game/UnkElemA0.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CRntString;

struct UnkElem80_7100778228 {
    u8 unk_0x0[0x80];
    ~UnkElem80_7100778228();  // fn_7100778228
};

struct UnkNamedElem38 {
    u64 field_0x0;
    CRntString name;
    u8 unk_end[0x38 - 0x8 - sizeof(CRntString)];
};

CRntVector<UnkElem80_7100778228>& fn_710077a478(CRntVector<UnkElem80_7100778228>* self,
                                               CRntVector<UnkElem80_7100778228>&& other) {
    return *self = static_cast<CRntVector<UnkElem80_7100778228>&&>(other);
}


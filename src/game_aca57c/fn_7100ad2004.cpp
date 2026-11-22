// Move-assignments of engine arrays (base/global/CRntVector.h): the elements are destroyed, then the
// other array's storage and settings are taken over. One element type per copy.
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "game/UnkElem138.h"
#include "game/UnkElem40.h"
#include "game/UnkElem80.h"
#include "game/UnkElemA0.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CRntString;

struct UnkNamedElem38 {
    u64 field_0x0;
    CRntString name;
    u8 unk_end[0x38 - 0x8 - sizeof(CRntString)];
};

CRntVector<UnkElem80>& fn_7100ad2004(CRntVector<UnkElem80>* self, CRntVector<UnkElem80>&& other) {
    return *self = static_cast<CRntVector<UnkElem80>&&>(other);
}
CRntVector<UnkNamedElem38>& fn_7100b1b2f8(CRntVector<UnkNamedElem38>* self, CRntVector<UnkNamedElem38>&& other) {
    return *self = static_cast<CRntVector<UnkNamedElem38>&&>(other);
}
CRntVector<UnkElemA0>& fn_7100c8ef88(CRntVector<UnkElemA0>* self, CRntVector<UnkElemA0>&& other) {
    return *self = static_cast<CRntVector<UnkElemA0>&&>(other);
}
CRntVector<UnkElem40>& fn_7100c92464(CRntVector<UnkElem40>* self, CRntVector<UnkElem40>&& other) {
    return *self = static_cast<CRntVector<UnkElem40>&&>(other);
}
CRntVector<UnkElem138>& fn_7100e624d8(CRntVector<UnkElem138>* self, CRntVector<UnkElem138>&& other) {
    return *self = static_cast<CRntVector<UnkElem138>&&>(other);
}

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

struct UnkElem40 {
    u8 unk_0x0[0x40];
    ~UnkElem40();  // fn_7100c8a490
};

struct UnkElem138 {
    u8 unk_0x0[0x138];
    ~UnkElem138();  // fn_7100e4e310
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

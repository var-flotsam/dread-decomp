// Destructor of a record of three string ids and a string (base::global::CRntString's layout,
// as UnkString30_133be4 in src/game/GUI/IDisplayObjectTrack/fn_7100b1ae74.cpp), which is cleared (fn_710007ed40) before
// the members are destroyed: fn_7100133be4.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using namespace base::global;

struct UnkString30_133be4 {
    u64 field_0x0[6];

    __attribute__((always_inline)) ~UnkString30_133be4() { CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(this)); }
};

struct UnkIdsString {
    UnkStrIdTwoStep field_0x0;
    UnkStrIdTwoStep field_0x8;
    UnkStrIdTwoStep field_0x10;
    UnkString30_133be4 field_0x18;

    __attribute__((always_inline)) ~UnkIdsString() { CRntString_Clear(reinterpret_cast<CRntString*>(&field_0x18)); }
};

void fn_7100133be4(UnkIdsString* self) { self->~UnkIdsString(); }

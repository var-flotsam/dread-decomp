// Destructor of a record of a string id, 8 bytes, two string ids and a string, as fn_7100133be4
// (src/base/tunable/CTunable/fn_7100133be4.cpp): fn_7100e6c960.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU2564.h"
#include "types.h"

using namespace base::global;

struct UnkString30 {
    u64 field_0x0[6];

    __attribute__((always_inline)) ~UnkString30() { fn_710007ecd4(reinterpret_cast<CRntString*>(this)); }
};

struct UnkIdsString {
    UnkStrIdTwoStep field_0x0;
    u64 field_0x8;
    UnkStrIdTwoStep field_0x10;
    UnkStrIdTwoStep field_0x18;
    UnkString30 field_0x20;

    __attribute__((always_inline)) ~UnkIdsString() { fn_710007ed40(reinterpret_cast<CRntString*>(&field_0x20)); }
};

void fn_7100e6c960(UnkIdsString* self) { self->~UnkIdsString(); }

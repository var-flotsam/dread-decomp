#pragma once

#include "game/UnkTU2566.h"
#include "types.h"

// Copy-assignment of two ids and a value.
struct UnkIdPairCopy {
    base::global::CStrId first;
    base::global::CStrId second;
    u32 field_0x10;
};

#pragma once

#include "base/reflection/CClass.h"
#include "types.h"

struct UnkCentralUnitEntry {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};

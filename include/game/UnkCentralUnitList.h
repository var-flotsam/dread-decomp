#pragma once

#include "types.h"

struct UnkCentralUnitEntry;

struct UnkCentralUnitList {
    UnkCentralUnitEntry** data;
    u32 count;
};

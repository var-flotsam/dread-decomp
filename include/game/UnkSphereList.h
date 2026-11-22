#pragma once

#include "types.h"

struct UnkSphereEntry;

struct UnkSphereList {
    UnkSphereEntry** data;
    u32 count;
};

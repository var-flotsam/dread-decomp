#pragma once

#include "types.h"

struct UnkRegistryDesc;

// The descriptor and its factory, set together (and copied as one block).
struct UnkRegistryEntryHead {
    UnkRegistryDesc* desc;
    void* factory;
};

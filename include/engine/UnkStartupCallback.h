#pragma once

#include "types.h"

// A callback and the object it is for (src/engine/HostArguments.cpp).
// A callback and the object it is for (none here).
struct UnkStartupCallback {
    void* object;
    void (*function)();
};

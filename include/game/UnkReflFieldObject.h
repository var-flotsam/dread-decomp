#pragma once

#include "types.h"

// An object and its type, as the walk passes it on.
struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

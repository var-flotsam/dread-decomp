#pragma once

#include "base/UnkStrIdPathPair.h"
#include "types.h"

// The id and path pair built from a resource's path, destroyed out of line.
// The id and path pair built from a path, destroyed out of line.
// The id and path pair, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

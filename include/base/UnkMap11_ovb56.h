#pragma once

#include "base/UnkChainedMap.h"

// As src/base/reflection/reflection_environment_ovb0056.cpp defines it: a chained map started with 11
// buckets.
// A chained map started with 11 buckets.
struct UnkMap11_ovb56 : UnkChainedMap {
    __attribute__((always_inline)) UnkMap11_ovb56() { ResizeBuckets(11); }
};

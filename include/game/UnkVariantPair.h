#pragma once

#include "types.h"

// Sets the variant at 0x10 from the pointer at 8: a pair of this and the pointer (state 2), or
// empty without one (fn_7100c051e8 and its copy).
// Sets the variant at 0x10 from the pointer at 8: a pair of this and the pointer (state 2), or
// empty without one (fn_7100c0621c and its copy).
struct UnkVariantPair {
    char* owner;
    void* value;
};

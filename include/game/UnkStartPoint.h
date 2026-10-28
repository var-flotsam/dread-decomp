#pragma once

// A global record of where the game starts: a character and a start point by name ("samus",
// "StartPoint0") and three other strings, built by unit 2's static initialiser (fn_7100001760,
// src/base/global/fn_7100001570.cpp) at 0x7101d473c0 and destroyed at exit by fn_7100000584
// (src/engine/HostArguments.cpp, unit 0). Names and layout are placeholders.

#include "base/global/CRntString.h"
#include "types.h"

struct UnkStartPoint {
    u32 field_0x0;
    bool field_0x4;
    base::global::CRntString field_0x8;
    base::global::CRntString field_0x38;
    base::global::CRntString character;    // +0x68: "samus"
    base::global::CRntString start_point;  // +0x98: "StartPoint0"
    base::global::CRntString field_0xc8;
};
static_assert(sizeof(UnkStartPoint) == 0xf8, "UnkStartPoint layout");

// Destroys the five strings, last to first.
void fn_7100000584(UnkStartPoint* self);

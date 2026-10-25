#pragma once

// A reference-counted resource, as its holders see it: a polymorphic object whose descriptor at
// +8 is its type in the resource registry (g_unk_7101d4bbc0, src/game_s7/fn_7100d92158.cpp) and
// whose count is at +0x54. A holder increments the count when it takes a reference and calls
// fn_710001737c when its decrement reaches zero. The count is a union member: the original has
// no type-based alias information for it (it reloads a holder's pointer after the increment).
// Only the count is spelled out; the name and the layout are placeholders.

#include "types.h"

struct UnkCountedResource {
    u8 unk_0x0[0x54];
    union {
        s32 ref_count;
    };
};
static_assert(sizeof(UnkCountedResource) == 0x58, "UnkCountedResource placeholder layout");

// Frees a resource whose last reference was dropped: fn_7100020468 looks the resource's descriptor
// up in the registry and hands the resource to the matching entry's factory (its vf_0x28). The
// definition is generated (src/lifted/).
void fn_710001737c(UnkCountedResource* resource);

#pragma once

#include "types.h"

// The sum of the two angles at 0xc8 of the argument's object, in degrees.
struct UnkAngleSource {
    u8 unk_0x0[0xc8];
    f32 field_0xc8;
    f32 field_0xcc;
};

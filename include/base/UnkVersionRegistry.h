#pragma once

#include "types.h"

// base::reflection's registry (0x1a8 bytes; unit 83): the settings' first word goes to +0x178
// before it is set up.
struct UnkVersionRegistry {
    u8 unk_0x0[0x178];
    u64 field_0x178;
    u8 unk_0x180[0x28];
};

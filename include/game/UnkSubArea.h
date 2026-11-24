#pragma once

#include "base/global/CStrId.h"
#include "types.h"

// A sub-area (a placeholder for the fields used here): two ids and a flag.
// A sub-area (a placeholder for the fields used here): its first id.
// A sub-area (a placeholder for the fields used here): two ids.
struct UnkSubArea {
    base::global::CStrId field_0x0;
    u8 pad_0x8[0x30];
    u8 field_0x38[0x18];  // passed to fn_7100ee8044 as the previous sub-area's part
    base::global::CStrId field_0x50;
    u8 pad_0x58[0x129];
    // A union member (no type-based alias info): the original reloads the manager's sub-area
    // pointers after storing it, and the flag itself after every store.
    union {
        bool field_0x181;
    };
};

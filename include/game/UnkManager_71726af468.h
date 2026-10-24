#pragma once

// A lazily created 0x2610-byte manager (fn_7100da7e2c constructs it): actors whose AI component
// has a flag set are handed to it (fn_71003609b8), and components register their id with the
// registry at its +0x330 (fn_71003f8210, game_s2). Reached through the GOT.

#include "types.h"

struct UnkManager_71726af468 {
    u8 pad_0x0[0x330];
    u8 field_0x330[0x2610 - 0x330];  // the registry at +0x330
};
extern UnkManager_71726af468* g_unk_71726af468;  // the manager instance
void fn_7100da7e2c(UnkManager_71726af468* self);  // constructor

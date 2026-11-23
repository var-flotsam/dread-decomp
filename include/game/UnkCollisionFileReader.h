#pragma once

#include "types.h"

// The collision file's reader (0x18 bytes; its constructor and destructor), and the read that
// fills the records (src/game/CCollisionComponent's unit).
// The collision file's reader (as src/game/CSubAreaManager.cpp declares it).
struct UnkCollisionFileReader {
    u8 unk_0x0[0x18];
};

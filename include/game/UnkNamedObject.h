#pragma once

#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

// Named objects (a CRntString at +8), in a list at +0x478.
struct UnkNamedObject {
    u8 pad_0x0[0x8];
    base::global::CRntString name;
};

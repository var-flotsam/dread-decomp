#pragma once

#include "game/UnkActor.h"
#include "game/UnkTU2661.h"

struct UnkCutsceneComponent_cs {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x38];
    u8 field_0x58[0x1a8];
    UnkCutsceneObject_cs* object;  // 0x200
};

#pragma once

#include "game/UnkNavComponent.h"
#include "types.h"

struct UnkNavActor {
    u8 pad_0x0[0x240];
    UnkNavComponent* component;
};

#pragma once

#include "game/UnkLightNode.h"
#include "game/UnkSpotLightBase.h"
#include "game/UnkSpotLightResRef.h"

// The 0x140-byte scene object.
struct UnkSpotLightObject {
    void* vtable;
    UnkSpotLightBase base;          // 0x8
    UnkSpotLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;              // 0x20
    u8 pad_0x28[0x70];
    UnkSpotLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};

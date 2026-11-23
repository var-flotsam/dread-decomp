#pragma once

#include "game/UnkDirLightBase.h"
#include "game/UnkDirLightResRef.h"
#include "game/UnkLightNode.h"

// The 0x140-byte scene object.
struct UnkDirLightObject {
    void* vtable;
    UnkDirLightBase base;          // 0x8
    UnkDirLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;             // 0x20
    u8 pad_0x28[0x70];
    UnkDirLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};

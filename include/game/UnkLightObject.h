#pragma once

#include "game/UnkLightBase.h"
#include "game/UnkLightNode.h"
#include "game/UnkLightResRef.h"

// The 0x220-byte scene object.
struct UnkLightObject {
    void* vtable;
    UnkLightBase base;          // 0x8
    UnkLightResRef resource;    // 0x10
    u8 pad_0x18[0x10];
    UnkLightNode node;          // 0x28
    u8 pad_0x30[0x70];
    UnkLightResRef resource2;   // 0xa0
    u8 pad_0xa8[0x178];
};

#pragma once

#include "game/UnkSegmentLightBase.h"
#include "game/UnkSegmentLightNode.h"
#include "game/UnkSegmentLightResRef.h"

// The 0x140-byte scene object.
struct UnkSegmentLightObject {
    void* vtable;
    UnkSegmentLightBase base;            // 0x8
    UnkSegmentLightResRef resource;      // 0x10
    u8 pad_0x18[0x8];
    UnkSegmentLightNode node;            // 0x20
    u8 pad_0x28[0x70];
    UnkSegmentLightResRef resource2;     // 0x98
    u8 pad_0xa0[0xa0];
};

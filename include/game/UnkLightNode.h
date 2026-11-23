#pragma once

#include "types.h"

// The light objects, as the light components' files define them (src/game/CDirLightComponent.cpp,
// src/game/CSpotLightComponent.cpp, src/game/COmniLightComponent.cpp,
// src/game/CSegmentLightComponent.cpp).
// The scene object's part at +0x20 with a virtual update (as COmniLightComponent.cpp's).
// The scene object's part at +0x20 with a virtual update (as the other light components').
// The scene object's part at +0x28 (constructed by fn_710020c38c) with a virtual update.
struct UnkLightNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

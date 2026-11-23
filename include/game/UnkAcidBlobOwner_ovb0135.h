#pragma once

#include "game/CFXComponent.h"
#include "game/UnkXformNode.h"

struct UnkAcidBlobOwner_ovb0135 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38: the position is its translation
    u8 pad_0x94[0x1ec];
    UnkObject* model;  // 0x280
    u8 pad_0x288[0x138];
    bool dirty;  // 0x3c0

    UnkXform* Xform() { return &xform; }
};

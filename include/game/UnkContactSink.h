#pragma once

#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

struct UnkContactHit;

struct UnkContactSink {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkColliderResult vf_0x10(UnkContactHit hit);
};

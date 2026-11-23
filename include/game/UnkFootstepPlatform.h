#pragma once

#include "types.h"

struct UnkFootstepArg;

struct UnkFootstepPlatform {
    virtual void vf_0x0();
    void Dispatch(UnkFootstepArg* arg) {
        reinterpret_cast<void (*)(UnkFootstepPlatform*, UnkFootstepArg*)>((*reinterpret_cast<void***>(this))[0x168 / 8])(this, arg);
    }
    u8 pad_0x8[0xc0];
    bool field_0xc8;
};

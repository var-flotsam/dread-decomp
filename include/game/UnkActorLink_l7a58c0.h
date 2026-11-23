#pragma once

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"

// A lazily resolved reference to an actor: the reflected value naming it, the state (1: not yet resolved,
// 2: resolved) and the handle.
struct UnkActorLink_l7a58c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    UnkErasedVector source;     // 0x8
    u32 state;                  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38
};

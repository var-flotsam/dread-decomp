#pragma once

#include "game/UnkAnimPose_ovb0125.h"

struct UnkAnimEvalNode_ovb0125 {
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
    virtual void Evaluate(UnkAnimPose_ovb0125* pose, void* arg);  // 0x78
    u8 pad_0x8[0x5c];
    f32 weight;  // 0x64
};

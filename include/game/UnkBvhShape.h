#pragma once

#include "game/UnkBvhBox.h"

struct UnkBvhShape {
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
    virtual void RefreshBounds();  // vf_0x98
    u8 unk_0x8[0x38];
    UnkBvhBox bounds;  // 0x40
    u8 unk_0x50[0x10];
    bool boundsDirty;  // 0x60
};

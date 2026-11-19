#pragma once

// A point the E.M.M.I.'s AI places relative to itself (unit 811, fn_7100551e4c and fn_7100551f0c): a base
// point from unit 815's table scaled by a value of the object at +0x29ee0, with w = 1. Names and layouts
// are placeholders.
#include "types.h"

struct UnkEmmyScaleSource {
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
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual struct UnkEmmyScaleValue* vf_0xd8();
};

struct UnkEmmyScaleValue {
    u8 pad_0x0[0x34];
    f32 scale;
};

struct UnkEmmyPoint4 {
    f32 x, y, z, w;
    UnkEmmyPoint4(f32 x_, f32 y_, f32 z_) {
        w = 1.0f;
        x = x_;
        y = y_;
        z = z_;
    }
    UnkEmmyPoint4(const UnkEmmyPoint4& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkEmmyScaledAI {
    u8 pad_0x0[0x29ee0];
    UnkEmmyScaleSource* source;
};

struct UnkEmmyBasePoint {
    f32 x, y, z;
};

// One of the E.M.M.I.'s base points (unit 815's table).
const UnkEmmyBasePoint* fn_7100572110(u32 index);

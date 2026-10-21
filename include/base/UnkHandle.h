#pragma once

// An object that holds another through a pointer at +0x18 and releases it on destruction
// (vtable via GOT 0x7101cbc318: destructors fn_7100084260/fn_71000842bc, then fn_710008434c,
// fn_7100084314, fn_7100084364, fn_7100084374). Names and layout are placeholders inferred
// from the code.

#include "types.h"

namespace base {

// Returned in memory (x8): more than 16 bytes; the contents are unknown.
struct UnkHandleInfo {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkHandleTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10();
    virtual void vf_0x18();
    virtual UnkHandleInfo vf_0x20();
};

struct UnkHandle {
    ~UnkHandle();

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10();  // fn_710008434c: whether the target is set and reports true
    virtual void vf_0x18();  // fn_7100084314: releases the target through the owner
    virtual UnkHandleInfo vf_0x20();  // fn_7100084364: forwards to the target

    void* owner;  // +0x8
    u64 field_0x10;
    UnkHandleTarget* target;  // +0x18
};

inline UnkHandle::~UnkHandle() {
    if (target && target->vf_0x10())
        vf_0x18();
}

}  // namespace base

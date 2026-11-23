#pragma once

#include "game/UnkBuckets.h"

template <class Tag>
struct UnkResourceMap {
    virtual UnkShared54Ref vf_0x0(const UnkResourceKey& key, u32 arg, void* data);  // find
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual UnkCountedResource* vf_0x48(const UnkResourceKey& key, u32 arg, void* data);

    UnkBuckets buckets;
};

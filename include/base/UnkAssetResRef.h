#pragma once

// A counted reference to a resource, as the resource registry returns it (fn_710002038c,
// fn_7100017878): a resource whose last reference goes is released through fn_710001737c. The unit
// that holds fn_710001737c (src/base/core/CAsset/UnkAssetResolve_b53.cpp) writes it out with that
// call inlined. Names are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkAssetResRef {
    UnkCountedResource* ptr;

    UnkAssetResRef() : ptr(nullptr) {}
    UnkAssetResRef& operator=(const UnkAssetResRef& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

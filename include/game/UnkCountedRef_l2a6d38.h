#pragma once

#include "base/UnkCountedResource.h"

// A counted reference whose move constructor starts empty and takes a new reference to the other's
// resource (the other keeps its own until it is destroyed).
struct UnkCountedRef_l2a6d38 {
    UnkCountedResource* ptr;

    UnkCountedRef_l2a6d38(UnkCountedRef_l2a6d38&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkCountedRef_l2a6d38() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

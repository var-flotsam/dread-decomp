#pragma once

#include "base/UnkCountedResource.h"

// As src/base/UnkUnit27/UnkCountedRefVector_Reserve_b53.cpp defines it.
// A counted reference whose move constructor starts empty and takes a new reference to the other's
// resource (the other keeps its own until it is destroyed).
struct UnkCountedRef_b53 {
    UnkCountedResource* ptr;

    UnkCountedRef_b53(UnkCountedRef_b53&& other) {
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
    ~UnkCountedRef_b53() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

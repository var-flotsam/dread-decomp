#pragma once

#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkShared54Ptr {
    UnkCountedResource* ptr;
    __attribute__((always_inline)) ~UnkShared54Ptr() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
    UnkShared54Ptr(const UnkShared54Ptr& other) {
        ptr = nullptr;
        *this = other.ptr;
    }
    UnkShared54Ptr& operator=(UnkCountedResource* p) {
        if (p != ptr) {
            if (p)
                ++p->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
        return *this;
    }
};

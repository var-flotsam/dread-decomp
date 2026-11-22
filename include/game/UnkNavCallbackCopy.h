#pragma once

#include "types.h"

// A callback passed by value: the original copies it member by member (a user-provided copy), so
// the copies go in memory and their addresses are passed.
struct UnkNavCallbackCopy {
    void* ctx;
    void* fn;

    template <class T>
    explicit UnkNavCallbackCopy(const T& other) {
        ctx = other.ctx;
        fn = reinterpret_cast<void*>(other.fn);
    }
    template <class T>
    const T& As() const {
        return *reinterpret_cast<const T*>(this);
    }
};

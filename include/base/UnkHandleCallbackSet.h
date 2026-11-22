#pragma once

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"

// Three values and a callback set whose owner is a handle assigned by fn_7100234f78.
struct UnkHandleCallbackSet {
    void* vtable;
    UnkErasedVector callbacks;
    u32 field_0x30;
    base::UnkRefHandle owner;

    __attribute__((always_inline)) UnkHandleCallbackSet& operator=(const UnkHandleCallbackSet& other) {
        const base::UnkRefHandle* from = &other.owner;
        fn_7100234f78(&owner, from);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }
};

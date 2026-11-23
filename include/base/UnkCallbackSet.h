#pragma once

#include "base/UnkValuesCallbacks.h"
#include "types.h"

// A callback set (as in src/game/CCharClassShootActivatorComponent/fn_71008947b8.cpp): a vtable, an array assigned by
// fn_710023b29c, a value and an owner reference assigned by fn_710031d208.
struct UnkCallbackSet {
    void* vtable;
    UnkErasedVector callbacks;
    u32 field_0x30;
    base::UnkRefHandle owner;

    __attribute__((always_inline)) UnkCallbackSet& operator=(const UnkCallbackSet& other) {
        const base::UnkRefHandle* from = &other.owner;
        fn_710031d208(&owner, from);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }
};

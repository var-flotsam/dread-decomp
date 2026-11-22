#pragma once

#include "types.h"

template <class Tag>
struct UnkSlot {
    union {  // (the original reloads the array after each null store, as through a union)
        void* ptr;
    };

    UnkSlot() : ptr(nullptr) {}
    UnkSlot& operator=(const UnkSlot& other);  // fn_71002a5d1c, fn_710031d208
};

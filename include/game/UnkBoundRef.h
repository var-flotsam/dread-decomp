#pragma once

#include "game/UnkBoundPair.h"

template <class Y>
struct UnkBoundRef {
    u32 value;
    Y* ptr;
    UnkBoundPair<u32, Y> pair;

    __attribute__((always_inline)) explicit UnkBoundRef(Y* p) : value(0), ptr(p) {
        pair.state = 0;
        if (ptr)
            pair.Set(&value, ptr);
    }
    __attribute__((always_inline)) UnkBoundRef& operator=(const UnkBoundRef& other) {
        if (this != &other) {
            value = other.value;
            ptr = other.ptr;
            if (ptr)
                pair.Set(&value, ptr);
            else
                pair.Reset();
        }
        return *this;
    }
    __attribute__((always_inline)) ~UnkBoundRef() { pair.Reset(); }
};

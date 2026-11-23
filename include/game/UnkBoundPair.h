#pragma once

#include "types.h"

template <class X, class Y>
struct UnkBoundPair {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;

    __attribute__((always_inline)) UnkBoundPair() {}
    __attribute__((always_inline)) ~UnkBoundPair() {}

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            values.y.~Y();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    __attribute__((always_inline)) void Set(X* x, Y* y) {
        Reset();
        state = 2;
        refs.x = x;
        refs.y = y;
    }
};

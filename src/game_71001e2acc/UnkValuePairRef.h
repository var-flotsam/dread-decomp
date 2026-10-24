// Pairs of values held either in place or by reference (state 2) and their assignments, as in
// src/game_s7/fn_71000d1fec.cpp (whose header-less copy this is, for this folder's copies).
#pragma once
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

template <class X, class Y>
struct UnkValuePairRef {
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

    X& GetX() { return state == 2 ? *refs.x : values.x; }
    Y& GetY() { return state == 2 ? *refs.y : values.y; }

    // Destroys the in-place values (state 1); either way the pair is left empty.
    void Reset() {
        switch (state) {
        case 1:
            values.y.~Y();
            values.x.~X();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    // Refers to `x` and `y`.
    void Set(X* x, Y* y) {
        Reset();
        state = 2;
        refs.x = x;
        refs.y = y;
    }
};

// Assigns both values whatever the other's state.
template <class X, class Y>
static inline UnkValuePairRef<X, Y>* Assign(UnkValuePairRef<X, Y>* self, UnkValuePairRef<X, Y>* other) {
    if (self != other) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

// Assigns only from an in-place (state 1) or by-reference (state 2) pair.
template <class X, class Y>
static inline UnkValuePairRef<X, Y>* AssignSet(UnkValuePairRef<X, Y>* self, UnkValuePairRef<X, Y>* other) {
    if (self != other) {
        switch (other->state) {
        case 1:
            self->GetX() = other->values.x;
            self->GetY() = other->values.y;
            break;
        case 2:
            self->GetX() = *other->refs.x;
            self->GetY() = *other->refs.y;
            break;
        }
    }
    return self;
}

// A value and a flag, and two values, assigned member by member.
struct UnkValueFlag {
    u32 value;
    bool flag;

    // Copies the five data bytes (not the padding).
    UnkValueFlag& operator=(const UnkValueFlag& other) {
        __builtin_memcpy(this, &other, 5);
        return *this;
    }
};

struct UnkValues2Memberwise {
    u32 a;
    u32 b;

    UnkValues2Memberwise& operator=(const UnkValues2Memberwise& other) {
        a = other.a;
        b = other.b;
        return *this;
    }
};

// Two values.
struct UnkValues2 {
    u32 a;
    u32 b;
};


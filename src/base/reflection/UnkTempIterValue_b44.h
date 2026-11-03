#pragma once

// An iterator value (base::UnkIterValue's layout: an owned (index, element) pair or a reference to
// one, and a state: 0 empty, 1 owns, 2 refers) that the reflected keyed collections' Begin and End
// slots (CCollectionType +0x160, +0x168) build as a temporary and move-assign to the output. Because
// the owned-pair branch of the assignment passes the temporary's address to an out-of-line
// constructor, the temporary stays on the stack: its reference is reloaded after the output's
// owned pair is destroyed, as in the original. Names and roles are placeholders.

#include <new>

#include "types.h"

template <class T, class K>
struct UnkTempIterValue {
    struct Value {
        K index;
        T element;
    };
    struct Ref {
        const K* index;
        T* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;

    UnkTempIterValue(const K* k, T* e) : state(2) {
        ref.index = k;
        ref.element = e;
    }
    ~UnkTempIterValue() { Reset(); }

    void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            value.~Value();
        } else {
            return;
        }
        state = 0;
    }

    UnkTempIterValue& operator=(UnkTempIterValue&& other) {
        Reset();
        state = other.state;
        if (state == 1)
            new (&value) Value{static_cast<K&&>(other.value.index), static_cast<T&&>(other.value.element)};
        else if (state == 2)
            ref = other.ref;
        return *this;
    }
};

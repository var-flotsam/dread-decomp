#pragma once

// The reflected begin (+0x160) and end (+0x168) slots of the CStrId-keyed flat maps in units 2563-2564
// and their copies: a (keys, values, count) map whose iterator is the cached (key, value) holder itself,
// empty, owning a copy, or referring to a key and a value. The slot builds a referring temporary and
// moves it into the output with the full state switch. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

template <class E>
struct UnkStrIdIterValue_A6_b75 {
    struct Value {
        base::global::CStrId key;
        E element;
    };
    struct Ref {
        base::global::CStrId* key;
        E* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;

    __attribute__((always_inline)) UnkStrIdIterValue_A6_b75(base::global::CStrId* k, E* e) {
        state = 2;
        ref.key = k;
        ref.element = e;
    }
    __attribute__((always_inline)) ~UnkStrIdIterValue_A6_b75() { Reset(); }

    __attribute__((always_inline)) void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            value.element.~E();
            value.key.Reset();
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) UnkStrIdIterValue_A6_b75& operator=(UnkStrIdIterValue_A6_b75&& o) {
        switch (state) {
        case 1:
            value.element.~E();
            value.key.Reset();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = o.state;
        ref = o.ref;
        o.Reset();
        return *this;
    }
};

template <class E>
struct UnkStrIdMap_A6_b75 {
    E* values;
    base::global::CStrId* keys;
    u32 count;
};

template <class E>
__attribute__((always_inline)) inline void UnkStrIdMapBegin_A6_b75(UnkStrIdMap_A6_b75<E>* self,
                                                                   UnkStrIdIterValue_A6_b75<E>* out) {
    *out = UnkStrIdIterValue_A6_b75<E>(self->keys, self->values);
}

template <class E>
__attribute__((always_inline)) inline void UnkStrIdMapEnd_A6_b75(UnkStrIdMap_A6_b75<E>* self,
                                                                 UnkStrIdIterValue_A6_b75<E>* out) {
    *out = UnkStrIdIterValue_A6_b75<E>(self->keys + self->count, self->values + self->count);
}

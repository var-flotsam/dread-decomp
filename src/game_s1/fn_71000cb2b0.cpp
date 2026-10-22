// Copies of fn_710105fec0 and fn_7100121728 (src/game_s7/fn_71000d1fec.cpp) for another value type, naming their own
// callees and data (the comments are adapted from the original's): fn_71000cb2b0, fn_710011feec; fn_71000cb35c,
// fn_710011ff98. Assignments of a pair of values held either in place or by reference (state 2), from another such
// pair: read and written through the state-dependent accessors.
#include "types.h"

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

// Three values, assigned member by member.
struct UnkValues3Memberwise {
    u32 a;
    u32 b;
    u32 c;

    UnkValues3Memberwise& operator=(const UnkValues3Memberwise& other) {
        a = other.a;
        b = other.b;
        c = other.c;
        return *this;
    }
};

using UnkU32Values3Pair = UnkValuePairRef<u32, UnkValues3Memberwise>;

UnkU32Values3Pair* fn_71000cb2b0(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return Assign(self, other); }
UnkU32Values3Pair* fn_710011feec(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return Assign(self, other); }

UnkU32Values3Pair* fn_71000cb35c(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return AssignSet(self, other); }
UnkU32Values3Pair* fn_710011ff98(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return AssignSet(self, other); }

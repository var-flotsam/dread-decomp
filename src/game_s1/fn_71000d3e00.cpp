// Copies of fn_7100a35c04 (src/game_s7/fn_71000d1fec.cpp) for a float second value, naming their own callees and data
// (the comments are adapted from the original's): fn_71000d3e00, fn_71000d4110. Equality of two pairs of values held
// either in place or by reference (state 2): both values compared through the state-dependent accessors.
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
};

template <class X, class Y>
static inline bool Equals(UnkValuePairRef<X, Y>* a, UnkValuePairRef<X, Y>* b) {
    return a->GetX() == b->GetX() && a->GetY() == b->GetY();
}

using UnkU32FloatPair = UnkValuePairRef<u32, f32>;

bool fn_71000d3e00(UnkU32FloatPair* a, UnkU32FloatPair* b) { return Equals(a, b); }
bool fn_71000d4110(UnkU32FloatPair* a, UnkU32FloatPair* b) { return Equals(a, b); }

// Identical copies: fn_7100dd0168, fn_7100dd0478.
bool fn_7100dd0168(UnkU32FloatPair* a, UnkU32FloatPair* b) { return Equals(a, b); }
bool fn_7100dd0478(UnkU32FloatPair* a, UnkU32FloatPair* b) { return Equals(a, b); }

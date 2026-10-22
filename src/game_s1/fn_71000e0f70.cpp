// Equality of a pair of values held either in place or by reference (state 2), as in
// fn_7100a35c04 (src/game_s7/fn_71000d1fec.cpp), for a 4-byte value and a 1-byte value:
// fn_71000e0f70, fn_71000e1280.
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

using UnkU32BytePair = UnkValuePairRef<u32, u8>;

// Both values compared through the state-dependent accessors.
static inline bool Equals(UnkU32BytePair* a, UnkU32BytePair* b) {
    return a->GetX() == b->GetX() && a->GetY() == b->GetY();
}

bool fn_71000e0f70(UnkU32BytePair* a, UnkU32BytePair* b) { return Equals(a, b); }

bool fn_71000e1280(UnkU32BytePair* a, UnkU32BytePair* b) { return Equals(a, b); }

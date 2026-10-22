// Assignment of a pair of string ids held either in place or by reference (state 2), from
// another such pair, as fn_7100e70ab4 (src/game_aca57c/fn_7100adae44.cpp) does for an id and four
// values: read and written through the state-dependent accessors: fn_71000d8a18.
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
};

using UnkIdIdPair = UnkValuePairRef<CStrId, CStrId>;

UnkIdIdPair* fn_71000d8a18(UnkIdIdPair* self, UnkIdIdPair* other) {
    if (other != self) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

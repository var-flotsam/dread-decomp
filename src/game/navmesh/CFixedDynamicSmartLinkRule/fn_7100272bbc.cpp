// A copy of fn_7100c0837c (src/base/reflection/fn_7100adae44.cpp) for another value type: the
// assignment of a pair held in place (state 1) or by reference (state 2) of a word and a record
// of four values, an array (assigned by fn_7100272348) and 0x15 plain bytes: fn_7100272bbc.
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

// base::UnkVector's layout.
struct UnkAssignedArray {
    u64 field_0x0[5];
};
void fn_7100272348(UnkAssignedArray* self, const UnkAssignedArray* other);

struct UnkPlainBytes_0x15 {
    u8 data[0x15];
};

struct UnkValuesArray {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    UnkAssignedArray array;
    UnkPlainBytes_0x15 tail;

    __attribute__((always_inline)) UnkValuesArray& operator=(const UnkValuesArray& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        fn_7100272348(&array, &other.array);
        tail = other.tail;
        return *this;
    }
};

using UnkU32ValuesArrayPair = UnkValuePairRef<u32, UnkValuesArray>;

// NON_MATCHING(minor): the original loads the tail's last 8 bytes before its first 16.
UnkU32ValuesArrayPair* fn_7100272bbc(UnkU32ValuesArrayPair* self, UnkU32ValuesArrayPair* other) {
    if (self != other) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

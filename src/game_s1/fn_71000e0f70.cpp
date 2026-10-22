// Equality and assignment of pairs of values held either in place or by reference (state 2), as
// in fn_7100a35c04 and fn_7100ffe094 (src/game_s7/fn_71000d1fec.cpp), for a 4-byte value and a
// 1-byte value (equality fn_71000e0f70, fn_71000e1280; assignments fn_71000e10a0, fn_71000e1140)
// or a 2-byte value (equality fn_71000d10d4, fn_71000d13e4; assignments fn_71000d1204,
// fn_71000d12a4). Also equality of a 4-byte value and a float vector compared within a tolerance:
// two components and a global tolerance (fn_7100121508, fn_710012187c), three components and 0.001
// (fn_71000cb0c8, fn_71000cb4dc).
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
template <class X, class Y>
static inline bool Equals(UnkValuePairRef<X, Y>* a, UnkValuePairRef<X, Y>* b) {
    return a->GetX() == b->GetX() && a->GetY() == b->GetY();
}

bool fn_71000e0f70(UnkU32BytePair* a, UnkU32BytePair* b) { return Equals(a, b); }

bool fn_71000e1280(UnkU32BytePair* a, UnkU32BytePair* b) { return Equals(a, b); }

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

using UnkU32HalfPair = UnkValuePairRef<u32, u16>;

UnkU32BytePair* fn_71000e10a0(UnkU32BytePair* self, UnkU32BytePair* other) { return Assign(self, other); }

UnkU32HalfPair* fn_71000d1204(UnkU32HalfPair* self, UnkU32HalfPair* other) { return Assign(self, other); }

UnkU32BytePair* fn_71000e1140(UnkU32BytePair* self, UnkU32BytePair* other) { return AssignSet(self, other); }

UnkU32HalfPair* fn_71000d12a4(UnkU32HalfPair* self, UnkU32HalfPair* other) { return AssignSet(self, other); }

bool fn_71000d10d4(UnkU32HalfPair* a, UnkU32HalfPair* b) { return Equals(a, b); }

bool fn_71000d13e4(UnkU32HalfPair* a, UnkU32HalfPair* b) { return Equals(a, b); }

// The tolerance of the two-component compare.
extern const f32 g_unk_7101647774;

struct UnkApproxVec2f {
    f32 x, y;

    bool operator==(const UnkApproxVec2f& o) const {
        return x - g_unk_7101647774 <= o.x && x + g_unk_7101647774 >= o.x && y - g_unk_7101647774 <= o.y &&
               y + g_unk_7101647774 >= o.y;
    }
};

struct UnkApproxVec3f {
    f32 x, y, z;

    bool operator==(const UnkApproxVec3f& o) const {
        return x - 0.001f <= o.x && x + 0.001f >= o.x && y - 0.001f <= o.y && y + 0.001f >= o.y &&
               z - 0.001f <= o.z && z + 0.001f >= o.z;
    }
};

using UnkU32Vec2Pair = UnkValuePairRef<u32, UnkApproxVec2f>;
using UnkU32Vec3Pair = UnkValuePairRef<u32, UnkApproxVec3f>;

bool fn_7100121508(UnkU32Vec2Pair* a, UnkU32Vec2Pair* b) { return Equals(a, b); }

bool fn_710012187c(UnkU32Vec2Pair* a, UnkU32Vec2Pair* b) { return Equals(a, b); }

bool fn_71000cb0c8(UnkU32Vec3Pair* a, UnkU32Vec3Pair* b) { return Equals(a, b); }

bool fn_71000cb4dc(UnkU32Vec3Pair* a, UnkU32Vec3Pair* b) { return Equals(a, b); }

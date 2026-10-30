// Assignments of pairs of a value and a 16- or 12-byte record held in place or by reference (as in
// fn_7100ffe094 and fn_7100ffe130, src/game_s7/fn_71000d1fec.cpp): fn_71002cb6b8, fn_71002cb754
// (16 bytes), fn_71003ed8fc, fn_71003ed9a0 (12 bytes).
#include "UnkValuePairRef.h"

struct UnkValues4 {
    u32 v[4];
};

struct UnkValues3 {
    u32 v[3];
};

using UnkU32Values4Pair = UnkValuePairRef<u32, UnkValues4>;
using UnkU32Values3Pair = UnkValuePairRef<u32, UnkValues3>;

UnkU32Values4Pair* fn_71002cb6b8(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return Assign(self, other); }

// Identical copies: fn_710066795c, fn_710075e9e8.
UnkU32Values4Pair* fn_710066795c(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return Assign(self, other); }
UnkU32Values4Pair* fn_710075e9e8(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return Assign(self, other); }

UnkU32Values4Pair* fn_71002cb754(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return AssignSet(self, other); }

// Identical copies: fn_71006679f8, fn_710075ea84.
UnkU32Values4Pair* fn_71006679f8(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return AssignSet(self, other); }
UnkU32Values4Pair* fn_710075ea84(UnkU32Values4Pair* self, UnkU32Values4Pair* other) { return AssignSet(self, other); }

UnkU32Values3Pair* fn_71003ed8fc(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return Assign(self, other); }

UnkU32Values3Pair* fn_71003ed9a0(UnkU32Values3Pair* self, UnkU32Values3Pair* other) { return AssignSet(self, other); }

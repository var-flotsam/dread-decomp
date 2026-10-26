#pragma once

// A 0x100-byte object (flags and counters, a string id at +0x38, four growable arrays at +0x58,
// +0x80, +0xa8 and +0xd0, and a refcounted pointer at +0xf8) that is copied and destroyed by two
// out-of-line routines: fn_71002b58c0 (copy constructor, src/game_lo/fn_71002b58c0.cpp) and
// fn_71002b6094 (destructor, src/game_lo/fn_71002b6094.cpp). Only the size is shared; each of
// those two files describes the members it touches.

#include "types.h"

struct UnkObject_0x100;
void fn_71002b58c0(UnkObject_0x100* self, const UnkObject_0x100* other);
void fn_71002b6094(UnkObject_0x100* self);

struct UnkObject_0x100 {
    u64 unk_0x0[0x20];

    __attribute__((always_inline)) UnkObject_0x100(const UnkObject_0x100& other) { fn_71002b58c0(this, &other); }
    __attribute__((always_inline)) ~UnkObject_0x100() { fn_71002b6094(this); }
};
static_assert(sizeof(UnkObject_0x100) == 0x100, "UnkObject_0x100 layout");

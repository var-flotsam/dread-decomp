#pragma once

// A 0x28-byte table of string ids kept in two arrays that share a count: keys (at 0) and ids (at
// 8). fn_71000df388, fn_7100ae2df8 and their copies release the ids and free both arrays; they
// are defined in src/game_s7/fn_710021c8dc.cpp and src/game_aca57c/fn_7100e4afd8.cpp.

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkKeyedStrIds {
    void* keys;
    base::UnkStrIdMember* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    // Takes over the arrays, leaving the other empty (a map moving its values into new storage).
    UnkKeyedStrIds(UnkKeyedStrIds&& other)
        : keys(other.keys), ids(other.ids), count(other.count), capacity(other.capacity),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), allocator(other.allocator) {
        other.count = 0;
        other.keys = nullptr;
        other.ids = nullptr;
    }
};
static_assert(sizeof(UnkKeyedStrIds) == 0x28, "UnkKeyedStrIds layout");

// Releases the ids, then frees the ids' array and the keys' array.
void fn_71000df388(UnkKeyedStrIds* self);
void fn_7100ae2df8(UnkKeyedStrIds* self);

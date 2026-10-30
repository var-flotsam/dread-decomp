#pragma once

// A list of file-path ids, laid out as an UnkVector of CFilePathStrId (base/UnkVector.h): the
// resource registry's records keep one (src/base/UnkResRegistry.cpp), and a sub-area change builds
// them (src/game/CSubAreaManager.cpp). Growing it is out of line (fn_7100018450); its copy
// constructor is inlined where it is used. The name is a placeholder.

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkResPathVec {
    base::global::CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkResPathVec(u32 flags)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags),
          allocator(fn_7100083990()) {}
    UnkResPathVec(const UnkResPathVec& other);  // inline in src/base/UnkResRegistry.cpp
};
static_assert(sizeof(UnkResPathVec) == 0x28, "UnkResPathVec layout");

// Grows the list to at least `capacity` ids, moving them (src/base/core/CAsset/fn_7100018450.cpp).
void fn_7100018450(UnkResPathVec* vec, u32 capacity);

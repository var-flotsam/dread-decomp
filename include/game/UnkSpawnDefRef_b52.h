#pragma once

#include "base/UnkCountedResource.h"
#include "types.h"

// A counted reference to a resource whose destructor reads the pointer once (as in
// src/game/CSpawnPointComponent/CSpawnPointComponent_b52.cpp).
// A counted reference like UnkResRef (base/UnkResKey.h) whose destructor reads the pointer once.
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

#pragma once

#include "game/UnkTU39.h"

// A counted reference to a resource.
struct UnkResourceRef_l1757f4 {
    UnkCountedResource* ptr;

    ~UnkResourceRef_l1757f4() {
        UnkCountedResource* resource = ptr;
        if (resource && --resource->ref_count == 0)
            fn_710001737c(resource);
    }
};

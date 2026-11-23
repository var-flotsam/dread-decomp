#pragma once

#include "base/UnkPathIdsFromText.h"

// A reference-counted resource (base/UnkCountedResource.h), dropped when the handle dies.
struct UnkResourceHandle {
    UnkCountedResource* ptr;

    ~UnkResourceHandle() {
        if (auto* held = ptr; held && --held->ref_count == 0)
            fn_710001737c(held);
    }
};

#pragma once

#include "base/UnkCountedResource.h"

// A counted resource reference (base/UnkCountedResource.h).
struct UnkSegmentLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

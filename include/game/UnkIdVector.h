#pragma once

#include "base/global/CRntVector.h"
#include "base/global/UnkTwoStepStrId.h"

struct UnkIdVector {
    UnkStrIdTwoStep* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkIdVector() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkStrIdTwoStep();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

#pragma once

#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "types.h"

struct UnkPathIdList_b0192 {
    base::global::CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

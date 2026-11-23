#pragma once

#include "game/UnkMusicEntry_b0019.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkArray;

struct UnkEntryArray_b0019 {
    UnkMusicEntry_b0019* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkMusicEntry_b0019& operator[](u64 i) {
        if (autoGrow && i >= count) {
            u32 n = capacity;
            if (i >= capacity && step != 0)
                n = ((u32)i / step + 1) * step;
            fn_7100fddcb4(reinterpret_cast<base::global::CRntVector<UnkMusicEntry_0x60>*>(this), n);
            fn_7100fde4d8(reinterpret_cast<UnkArray*>(this), i + 1);
        }
        return data[i];
    }
};

#pragma once

#include "game/UnkMusicEntry_b0019.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkArray_fdd2b4;

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
            CTunableMusicManager_ReserveRequestArray(reinterpret_cast<base::global::CRntVector<UnkMusicEntry_0x60>*>(this), n);
            CTunableMusicManager_ResizeRequestArray(reinterpret_cast<UnkArray_fdd2b4*>(this), i + 1);
        }
        return data[i];
    }
};

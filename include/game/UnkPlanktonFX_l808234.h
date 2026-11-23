#pragma once

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/UnkPlanktonEmitter_l808234.h"
#include "types.h"

struct UnkPlanktonFX_l808234 {
    void** vtable;
    u8 pad_0x8[0x40];
    base::global::CFilePathStrId path_0x48;
    UnkFixedVector_l808234<UnkPlanktonEmitter_l808234, 10> emitters;  // 0x50
    u8 pad_0x1274[0x20];
    f32 intensity;  // 0x1294
    base::global::CRntVector<u64> list_0x1298;
    UnkModel* model;  // 0x12c0
};

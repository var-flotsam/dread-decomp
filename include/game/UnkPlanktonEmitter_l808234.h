#pragma once

#include "game/UnkFixedVector_l808234.h"
#include "game/UnkModel.h"
#include "game/UnkPlanktonPoint_l808234.h"
#include "types.h"

struct UnkPlanktonEmitter_l808234 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    f32 field_0x10 = -1.0f;
    u32 field_0x14;  // unset in the original
    UnkModel* model = nullptr;  // 0x18
    u64 field_0x20 = 0;
    UnkFixedVector_l808234<UnkPlanktonPoint_l808234, 34> points;  // 0x28
    u32 field_0x1c4 = 0;
    bool field_0x1c8 = false;
    bool field_0x1c9 = false;
    bool field_0x1ca = false;

    UnkPlanktonEmitter_l808234() {}
};

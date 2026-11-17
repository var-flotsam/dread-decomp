#pragma once

// CLogicDroppedItemManager::CTunableDroppableBillboard's data (0x258 bytes, the tunable g_unk_71727c10c8) as its
// constructor and destructors (src/game/UnkTU2562/UnkTU2562_Billboard_ovb0208.cpp) and its unit's initialiser
// (fn_7100e1dde0) see it: the nine preset names from 0x98 (tools/gen_fields.py). Its vtable is
// g_unk_7101aa9cc8 (game/UnkTU2562.h). Names and layout are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkDropTunable_ovb0208 : UnkTunable {
    u8 pad_0x8[0x90];
    base::global::CRntString n0, n1, n2, n3, n4, n5, n6, n7, n8;  // 0x98
    u8 pad_0x248[0x10];
};
static_assert(sizeof(UnkDropTunable_ovb0208) == 0x258, "size");

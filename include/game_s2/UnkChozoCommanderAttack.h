#pragma once

// CChozoCommanderAttack, the base of the Chozo commander attack states (the unit that tools/tu_map.py
// puts fn_7100426b54 in): the owner actor sits at +0x8. The derived states pass themselves to the
// functions of the base unit. Only the members those functions touch are spelled out; the name and
// the layout are placeholders.

#include "game/UnkActor.h"
#include "types.h"

struct UnkChozoCommanderAttack {
    u8 pad_0x0[0x8];
    UnkActor* field_0x8;  // the owner
};
static_assert(__builtin_offsetof(UnkChozoCommanderAttack, field_0x8) == 0x8, "UnkChozoCommanderAttack placeholder layout");

// When the owner has the sub-object at +0x220, sends it a request under the ids `id` and
// 0x71b15e29dc542a11 with the mode 2 when `flag` is set, else 0. Defined in
// src/game/CChozoCommanderAttack/fn_7100426b54.cpp.
void fn_7100426b54(UnkChozoCommanderAttack* self, u64 id, bool flag);

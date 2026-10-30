#pragma once

// game::logic::collision::CBroadphaseObject's flag setter (src/game/logic/collision/CBroadphaseObject/
// fn_7100d35a98.cpp): a change of the flag at +0x6a runs the object's vf_0x98 and vf_0x48. The callers
// name the object UnkEntry, each with its own view of it.

#include "types.h"

struct UnkEntry;
void fn_7100d35a98(UnkEntry* entry, bool flag);

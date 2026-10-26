#pragma once

// An object with a vtable that keeps named entries in an array at +0x48 (0x30 bytes each: the
// name's string id first, a flag at +0x18, the value at +0x28), as fn_71008640a4 sees it: the
// lookup by name returns the entry's value, first refreshing the entry through vf_0x170 when its
// flag is set. The callers reach it through their owner's +0x2a0 and look components up by name
// ("damage"). tools/tu_map.py puts fn_71008640a4 in the unit it names after CCollisionComponent.
// The names and the layout are placeholders.

#include "base/global/CStrId.h"
#include "types.h"

struct UnkNamedComponents;
struct UnkNamedComponent;
UnkNamedComponent* fn_71008640a4(UnkNamedComponents* self, const base::global::CStrId* name);

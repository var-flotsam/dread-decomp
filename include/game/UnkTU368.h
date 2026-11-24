#pragma once

// Unit 368's shared declarations. Names are placeholders.

#include "types.h"

struct UnkQueryObj;

// Whether the object's query kind is the first kind selected in `mask`.
bool ObjectKindMatchesFirstSetMaskBit(UnkQueryObj* obj, u32 mask);

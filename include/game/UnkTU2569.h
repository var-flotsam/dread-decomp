#pragma once

// Unit 2569 of tools/tu_map.py. Declarations only; names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// The unit's 32-byte entry (src/game/UnkTU2569/UnkTU2569_Index_ovb0245.cpp defines it) and its array.
template <bool kSwapped>
struct UnkIdEntryT_ovb0245;
using UnkIdVec_ovb0245 = base::global::CRntVector<UnkIdEntryT_ovb0245<true>>;
// Shrinks the array to `n` entries.
void fn_7100ec4fd4(UnkIdVec_ovb0245* v, u32 n);

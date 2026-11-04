// Unit 311 of tools/tu_map.py (no reflected class): whether `flags` has every bit of `mask` (used
// by src/game_lo/fn_7100219ae0.cpp). Names are placeholders.
#include "types.h"

bool fn_7100217e50(u32 flags, u32 mask) { return (mask & ~flags) == 0; }

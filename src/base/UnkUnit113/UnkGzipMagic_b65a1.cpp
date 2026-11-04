// Unit 113 of tools/tu_map.py (no class hint): whether a buffer starts with the gzip magic
// (0x1f 0x8b, read as a little-endian halfword); false without a buffer. Names are placeholders.
#include "types.h"

bool fn_71001409f0(const u16* data) { return data && *data == 0x8b1f; }

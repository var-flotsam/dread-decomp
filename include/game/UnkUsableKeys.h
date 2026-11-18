#pragma once

// Read-only keys in .rodata (0x710164b5xx) that the usable components (access points, map stations,
// save stations) pass to their state requests; several files share each one. Names are placeholders.
#include "types.h"
extern const u8 g_unk_710164b540[] __attribute__((visibility("hidden")));
extern const u64 g_unk_710164b5a8;

// UnkTU177: sets a global write-mask flag; when it changes, marks the state dirty and calls a global
// function pointer for each of the eight slots 0-7 with the colour flag and the new value.
// Names and layouts are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU177.h"
#include "types.h"

extern bool g_unk_7101a1d7d9;
extern bool g_unk_7101a1d7da;
extern void (*g_unk_716e29b578)(void* object, s32 slot, bool red, bool green, bool blue, bool alpha);

void fn_710015a260(u32 value) {
    bool alpha = value & 1;
    if (g_unk_7101a1d7da == alpha)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7da = alpha;
    g_unk_7101a1d7d8 = true;
    bool color = g_unk_7101a1d7d9;
    for (s32 i = 0; i < 8; ++i)
        g_unk_716e29b578(g_unk_716e29c680, i, color, color, color, alpha);
}

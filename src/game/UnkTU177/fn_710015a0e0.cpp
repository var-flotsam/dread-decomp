#include "game/UnkGraphicsState.h"
#include "game/UnkTU177.h"
#include "types.h"

extern bool g_unk_7101a1d7d9;
extern bool g_unk_7101a1d7d8;
extern void (*g_unk_716e29b578)(void* target, s32 index, bool r, bool g, bool b, bool a);

// NON_MATCHING(minor): the same instructions in another order or with other registers.
void fn_710015a0e0(u32 value) {
    bool on = value & 1;
    if (g_unk_7101a1d7d9 == on)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7d9 = on;
    g_unk_7101a1d7d8 = true;
    for (s32 i = 0; i < 8; ++i)
        g_unk_716e29b578(g_unk_716e29c680, i, on, on, on, on);
}

// Sets a global flag; when it changes, also sets two other global flags and calls a global
// function pointer for each of the eight slots 0-7 of a global object with the new value.
#include "game/UnkGraphicsState.h"
#include "types.h"

void fn_710015a3e0(bool value) {
    if (g_unk_7101a1d7dc == value)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7db = true;
    g_unk_7101a1d7dc = value;
    for (s32 i = 0; i < 8; ++i)
        g_unk_716e29b540(g_unk_716e29c68c, i, value);
}

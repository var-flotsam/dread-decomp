// tools/tu_map.py unit 177 (the graphics state of game/UnkGraphicsState.h): sets a global mode; when
// it changes, marks two dirty flags and stores the mode's mapped value, which is then overwritten
// with 7 and, when the flag fn_710015a760 keeps is set, passed to the NVN callback it uses. Names are
// placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

extern s32 g_unk_716e29c688;
extern bool g_unk_716e29c684;
extern s32 g_unk_7101a1d7e0;
extern void (*g_unk_716e29b550)(void* object, s32 value);

void fn_710015a7d0(u32 mode) {
    if (g_unk_716e29c688 == (s32)mode)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7db = true;
    g_unk_716e29c688 = mode;
    switch (mode) {
    case 0:
        g_unk_7101a1d7e0 = 8;
        break;
    case 1:
        g_unk_7101a1d7e0 = 1;
        break;
    case 2:
        g_unk_7101a1d7e0 = 3;
        break;
    case 3:
        g_unk_7101a1d7e0 = 6;
        break;
    case 4:
        g_unk_7101a1d7e0 = 2;
        break;
    case 5:
        g_unk_7101a1d7e0 = 4;
        break;
    case 6:
        g_unk_7101a1d7e0 = 5;
        break;
    case 7:
        g_unk_7101a1d7e0 = 7;
        break;
    }
    g_unk_7101a1d7e0 = 7;
    if (g_unk_716e29c684)
        g_unk_716e29b550(g_unk_716e29c68c, 7);
}

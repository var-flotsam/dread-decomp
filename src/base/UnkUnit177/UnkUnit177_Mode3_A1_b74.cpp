// tools/tu_map.py unit 177 (the graphics state of game/UnkGraphicsState.h): sets another global mode; when it
// changes, marks two dirty flags, maps modes 0 and 1 to 2 and 1 (other modes keep the stored value) and passes
// the value to an NVN callback. Shaped like fn_710015a7d0 (UnkUnit177_Mode2_A1_b71.cpp). Names are placeholders.
#include "game/UnkGraphicsState.h"
#include "game/UnkTU176.h"
#include "game/UnkTU177.h"
#include "types.h"

extern s32 g_unk_716e29c6c8;
extern bool g_unk_7101a1d801;
extern s32 g_unk_7101a1d80c;

void fn_710015aa80(u32 mode) {
    if (g_unk_716e29c6c8 == (s32)mode)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d801 = true;
    g_unk_716e29c6c8 = mode;
    if (mode == 0)
        g_unk_7101a1d80c = 2;
    else if (mode == 1)
        g_unk_7101a1d80c = 1;
    g_unk_716e29b658(g_unk_716e29c6cc, g_unk_7101a1d80c);
}

// tools/tu_map.py unit 177 (the graphics state of game/UnkGraphicsState.h): sets a global flag; when
// it changes, marks two dirty flags and passes either a stored value or 8 to an NVN callback on a
// global object. Names are placeholders.
#include "game/UnkGraphicsState.h"
#include "types.h"

extern bool g_unk_716e29c684;
extern s32 g_unk_7101a1d7e0;
extern void (*g_unk_716e29b550)(void* object, s32 value);

void fn_710015a760(bool on) {
    if (g_unk_716e29c684 == on)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7db = true;
    g_unk_716e29c684 = on;
    s32 value = on ? g_unk_7101a1d7e0 : 8;
    g_unk_716e29b550(g_unk_716e29c68c, value);
}

// Sets two global mode values (0-9). When either changes, marks two flags dirty, records the
// new values, maps each into a second numbering (0-3 -> 1-4, 4-5 -> 9-10, 6-9 -> 5-8), and
// passes the mapped pair, twice, to a global callback.
#include "game/UnkGraphicsState.h"
#include "types.h"

static inline void Map_710015a500(s32 v, s32* out) {
    switch (v) {
    case 0:
        *out = 1;
        break;
    case 1:
        *out = 2;
        break;
    case 2:
        *out = 3;
        break;
    case 3:
        *out = 4;
        break;
    case 4:
        *out = 9;
        break;
    case 5:
        *out = 10;
        break;
    case 6:
        *out = 5;
        break;
    case 7:
        *out = 6;
        break;
    case 8:
        *out = 7;
        break;
    case 9:
        *out = 8;
        break;
    }
}

void fn_710015a500(s32 a, s32 b) {
    if (g_unk_7101a1d7c8 == a && g_unk_716e29c670 == b)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7c6 = true;
    if (g_unk_7101a1d7c8 != a) {
        g_unk_7101a1d7c8 = a;
        Map_710015a500(a, &g_unk_7101a1d7d0);
    }
    if (g_unk_716e29c670 != b) {
        g_unk_716e29c670 = b;
        Map_710015a500(b, &g_unk_7101a1d7d4);
    }
    s32 ma = g_unk_7101a1d7d0;
    s32 mb = g_unk_7101a1d7d4;
    g_unk_716e29b4d0(g_unk_716e29c678, ma, mb, ma, mb);
}

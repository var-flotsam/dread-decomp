// UnkTU177: sets three global operation values (0-7). When any changes, records them, maps each
// into a second numbering and passes the mapped triple to a global callback for two faces.
// Names and layouts are placeholders.
#include "game/UnkTU176.h"
#include "game/UnkTU177.h"
#include "types.h"

extern s32 g_unk_716e29c698;
extern s32 g_unk_716e29c69c;
extern s32 g_unk_716e29c6a0;

static inline void Map_710015afd0(s32 v, s32* out) {
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
        *out = 7;
        break;
    case 4:
        *out = 8;
        break;
    case 5:
        *out = 6;
        break;
    case 6:
        *out = 4;
        break;
    case 7:
        *out = 5;
        break;
    }
}

void SetStencilOpsIfChanged(s32 a, s32 b, s32 c) {
    if (g_unk_716e29c698 == a && g_unk_716e29c69c == b && g_unk_716e29c6a0 == c)
        return;
    g_unk_716e29c698 = a;
    g_unk_716e29c69c = b;
    g_unk_716e29c6a0 = c;
    Map_710015afd0(a, &g_unk_7101a1d7f4);
    Map_710015afd0(b, &g_unk_7101a1d7f8);
    Map_710015afd0(c, &g_unk_7101a1d7fc);
    for (s32 face = 2; face > 0; --face) {
        s32 fail = g_unk_7101a1d7f4;
        s32 depth_fail = g_unk_7101a1d7f8;
        s32 pass = g_unk_7101a1d7fc;
        g_unk_716e29b6b8(g_unk_716e29c6a8, face, fail, depth_fail, pass);
    }
}

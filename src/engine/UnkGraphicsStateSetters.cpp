// Render-state setters of the graphics unit whose start-up is src/game_s7/fn_71001587f0.cpp (its
// neighbours src/game_lo/fn_710015a3e0.cpp and fn_710015a500.cpp are the same kind): each keeps
// the current value in a global, and only when it changes marks the state dirty and passes the new
// value to the NVN entry point that fn_71001587f0 loaded.

#include "game/UnkGraphicsState.h"
#include "types.h"

// Two flags of the state object g_unk_716e29c6a8 and the entry points that set them (the names
// of src/game_s7/fn_71001587f0.cpp).
extern u8 g_unk_716e29c690;
extern u8 g_unk_7101a1d7e5;
extern u8 g_unk_716e29c6a8[];
extern void (*g_unk_716e29b690)(void* object, u8 value);
extern void (*g_unk_716e29b698)(void* object, u8 value);
extern bool g_unk_7101a1d7e4;  // the state object's dirty flag

// A mode of the target g_unk_716e29c678 (0-4), the value it maps to (1-5) and its entry point,
// which takes that value twice.
extern s32 g_unk_716e29c66c;
extern s32 g_unk_7101a1d7cc;
extern void (*g_unk_716e29b4d8)(void* target, s32 a, s32 b);

void fn_710015a860(u32 value) {
    if (g_unk_716e29c690 == (u8)(value & 1))
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7e4 = true;
    g_unk_716e29c690 = value & 1;
    g_unk_716e29b690(g_unk_716e29c6a8, value & 1);
}

void fn_710015aa20(u32 value) {
    if (g_unk_7101a1d7e5 == (u8)(value & 1))
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_7101a1d7e4 = true;
    g_unk_7101a1d7e5 = value & 1;
    g_unk_716e29b698(g_unk_716e29c6a8, value & 1);
}

// A mode outside 0-4 keeps the last mapped value.
void fn_710015a680(u32 value) {
    if (g_unk_716e29c66c == (s32)value)
        return;
    g_unk_7101a1d7c5 = true;
    g_unk_716e29c66c = value;
    g_unk_7101a1d7c6 = true;
    switch (value) {
    case 0:
        g_unk_7101a1d7cc = 1;
        break;
    case 1:
        g_unk_7101a1d7cc = 2;
        break;
    case 2:
        g_unk_7101a1d7cc = 3;
        break;
    case 3:
        g_unk_7101a1d7cc = 4;
        break;
    case 4:
        g_unk_7101a1d7cc = 5;
        break;
    }
    s32 mapped = g_unk_7101a1d7cc;
    g_unk_716e29b4d8(g_unk_716e29c678, mapped, mapped);
}

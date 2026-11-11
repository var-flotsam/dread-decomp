// Unit 2727 (near gui/scripts/msemenu_mainmenu.lua's strings): passes a value to a lazily created
// object (g_unk_71727bc260), to the GUI object (g_unk_7101d47330) while it answers fn_7101196fd8,
// and otherwise to the game manager (g_unk_7101d47310). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2538.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU2737.h"
#include "types.h"

struct UnkObj_71011a50cc {
    u8 unk_0x0[0x98];
    UnkObj_71011a50cc();  // fn_71011a50cc
};

extern void* g_unk_7101d47330;
void fn_7101197e2c(void* gui, f32 value);
void fn_7100dec1d8(UnkGameManager* self, f32 value, bool flag);

void fn_7101190784(f32 value) {
    if (!g_unk_71727bc260)
        g_unk_71727bc260 = new (fn_71000839a0(0x98, 0x3f)) UnkObj_71011a50cc();
    fn_71011a5798(g_unk_71727bc260, 0, value);
    if (fn_7101196fd8(g_unk_7101d47330))
        fn_7101197e2c(g_unk_7101d47330, value);
    if (fn_7101196fd8(g_unk_7101d47330))
        return;
    fn_7100dec1d8(g_unk_7101d47310, value, true);
}

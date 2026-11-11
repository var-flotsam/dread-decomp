// Unit 2727 (near gui/scripts/msemenu_mainmenu.lua's strings): unless the GUI object
// (g_unk_7101d47330) answers fn_7101196fd8, passes the value and a set flag to the game manager
// (g_unk_7101d47310) through fn_7100dec69c. Names are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2730.h"
#include "types.h"

extern void* g_unk_7101d47330;
void fn_7100dec69c(UnkGameManager* self, f32 value, bool flag);

void fn_7101190838(f32 value) {
    if (fn_7101196fd8(g_unk_7101d47330))
        return;
    fn_7100dec69c(g_unk_7101d47310, value, true);
}

// Update of a GUI screen that starts a new game when asked (flag at 0x2ac): unless
// fn_7101053a98 vetoes it, the screen's GUI key is dropped and the script function
// Init.CreateAndInitNewGame is called for the cave start point, then the request is cleared.
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

struct UnkNewGameScreen {
    u8 unk_0x0[0x2ac];
    bool field_0x2ac;
};

extern base::global::CStrId g_unk_71727659c0 __attribute__((visibility("hidden")));

void fn_71011d8fe4(UnkNewGameScreen* self, f32 dt);
bool fn_7101053a98(void* state);
s32 fn_7100dffc58(UnkGameManager* manager);
void fn_7100000790(const char* name, const char* format, ...);  // calls a script function

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100b6503c(UnkNewGameScreen* self, f32 dt) {
    fn_71011d8fe4(self, dt);
    if (self->field_0x2ac && !fn_7101053a98(g_unk_7101d47310->field_0x1040)) {
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_71727659c0);
        fn_7100000790("Init.CreateAndInitNewGame", "iss", fn_7100dffc58(g_unk_7101d47310), "s010_cave",
                      "StartPoint0");
        self->field_0x2ac = false;
    }
}

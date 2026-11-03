// Unit 1697 (tools/tu_map.py, no name hint): the counterpart of fn_7100b4fbd8
// (src/game/UnkUnit1697Fadescreen_b43.cpp): hands the target (+0x270) the composition's root, switches
// the root's "Fadescreen" child on (fn_71011cdce8), lets the target update (fn_7100b364dc) and turns the
// render manager's two fade settings on. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

// As src/game_aca57c/fn_7100b4c43c.cpp declares it.
struct UnkMenuTarget;
void fn_7100b364dc(UnkMenuTarget* self);

struct UnkFadeComposition_b49 {
    void** vtable;
    UnkGuiDisplayObject* root;  // 0x8
};

struct UnkFadeTarget_b49 {
    u8 pad_0x0[0x270];
    UnkGuiDisplayObject* root;  // 0x270
};

void fn_7100b4fcd4(UnkFadeComposition_b49* self, UnkFadeTarget_b49* target) {
    // Stored without the record's type: the original reloads the composition's root after it.
    UnkGuiDisplayObject** root = &target->root;
    *root = self->root;
    fn_71011cdce8(fn_71011d94d4(self->root, CStrId("Fadescreen", true)), true);
    fn_7100b364dc(reinterpret_cast<UnkMenuTarget*>(target));
    fn_7100219d4c(g_unk_7101d47300, true, 0);
    fn_7100219cb0(g_unk_7101d47300, true, 0);
}

// Unit 1697 (tools/tu_map.py, no name hint): sets up a screen fade: the target gets state 3 (+0x27c)
// and the composition's root object (+0x268; the composition is built on first use, its vf_0x10), the
// root's "Fadescreen" child is switched off (fn_71011cdce8), and the render manager's three fade
// settings are reset. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CStrId;

struct UnkFadeComposition_b43a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // builds the root
    UnkGuiDisplayObject* root;  // 0x8

    UnkGuiDisplayObject* Root() {
        if (!root)
            vf_0x10();
        return root;
    }
};

struct UnkFadeTarget_b43a3 {
    u8 pad_0x0[0x268];
    UnkGuiDisplayObject* root;  // 0x268
    u8 pad_0x270[0xc];
    u32 state;  // 0x27c
};

void fn_7100b4fbd8(UnkFadeComposition_b43a3* self, UnkFadeTarget_b43a3* target) {
    target->state = 3;
    // Stored without the record's type: the original reloads the composition's root after it.
    UnkGuiDisplayObject** root = &target->root;
    *root = self->Root();
    fn_71011cdce8(fn_71011d94d4(self->root, CStrId("Fadescreen", true)), false);
    fn_7100219d4c(g_unk_7101d47300, false, 0);
    fn_7100219cb0(g_unk_7101d47300, false, 0);
    fn_7100219da4(g_unk_7101d47300, true, true, 2);
}

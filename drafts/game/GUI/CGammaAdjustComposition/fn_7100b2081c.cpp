// GUI::CGammaAdjustComposition's unit (tools/tu_map.py 1678): after the base handling, on the input event of
// kind 2 asks "#GUI_GAMMA_LABEL_CONFIRM_OPTIONS" (with accept and back texts and the composition's two
// callbacks) on the main menu's pop-up, or in game on the GUI flag reference's composition, and marks the
// event handled. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;

struct UnkGammaAdjustComposition_lb2081c;
void fn_7100b210a0(UnkGammaAdjustComposition_lb2081c* self);
void fn_7100b210a4(UnkGammaAdjustComposition_lb2081c* self);

// A callback: an object and a function to call on it.
struct UnkGammaCallback_lb2081c {
    UnkGammaAdjustComposition_lb2081c* object;
    void (*function)(UnkGammaAdjustComposition_lb2081c*);
};

struct UnkGammaMainMenu_lb2081c {
    u8 pad_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

// The GUI flag reference of the game manager: its composition at +0x10.
struct UnkGammaGuiRef_lb2081c {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

struct UnkGammaInputEvent_lb2081c {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
    u8 pad_0x1b[5];
    u64 kind;      // 0x20
};

void fn_7100b2081c(UnkGammaAdjustComposition_lb2081c* self, UnkGammaInputEvent_lb2081c* event) {
    fn_71011cf70c(reinterpret_cast<UnkGallery_A5_b75*>(self));
    if (event->kind != 2)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    if (fn_7100deac24(g_unk_7101d47310)) {
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu && fn_710009d50c(menu->vf_0x0(), fn_7100b34230())) {
            UnkReflObject* popup = reinterpret_cast<UnkGammaMainMenu_lb2081c*>(menu)->popup;
            if (popup && fn_710009d50c(popup->vf_0x0(), fn_7100b5e368())) {
                CStrId text("#GUI_GAMMA_LABEL_CONFIRM_OPTIONS", true);
                UnkGammaCallback_lb2081c accept_callback = {self, fn_7100b210a0};
                UnkGammaCallback_lb2081c back_callback = {self, fn_7100b210a4};
                CPopUpComposition_ShowAcceptBackPrompt(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                              reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                              reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
            }
        }
    } else {
        UnkGammaGuiRef_lb2081c* ref = static_cast<UnkGammaGuiRef_lb2081c*>(g_unk_7101d47310->field_0x2258);
        if (ref && ref->composition) {
            CStrId text("#GUI_GAMMA_LABEL_CONFIRM_OPTIONS", true);
            UnkGammaCallback_lb2081c accept_callback = {self, fn_7100b210a0};
            UnkGammaCallback_lb2081c back_callback = {self, fn_7100b210a4};
            CPopUpComposition_ShowAcceptBackPrompt(ref->composition, &text, &accept, &back,
                          reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                          reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
        }
    }
    event->handled = true;
}

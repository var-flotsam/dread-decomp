// GUI::CExitToMainMenuButton's unit (tools/tu_map.py unit 1675): fn_7100b1ebf0 hides the in-game menu
// (fn_71011cf2a4) and asks "#GUI_OPTIONS_PROMPT_QUIT" ("#GUI_BOSSRUSH_PROMPT_QUIT_BOSSRUSH" in boss rush)
// through the GUI composition with accept and back texts and two callbacks on the button, as
// src/game/GUI/CLoadCheckPointButton/CLoadCheckPointButton_Prompt_ovb0118.cpp. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

struct UnkExitToMainMenuButton_ovb0136;
void fn_7100b1ef00(UnkExitToMainMenuButton_ovb0136* self);
void fn_7100b1ef58(UnkExitToMainMenuButton_ovb0136* self);

// A callback: an object and a function to call on it.
struct UnkButtonCallback_ovb0136 {
    UnkExitToMainMenuButton_ovb0136* object;
    void (*function)(UnkExitToMainMenuButton_ovb0136*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

struct UnkGuiRef_ovb0136 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

void fn_7100b1ebf0(UnkExitToMainMenuButton_ovb0136* self) {
    UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310);
    if (!menu)
        return;
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), false);
    UnkGuiRef_ovb0136* ref = static_cast<UnkGuiRef_ovb0136*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text(g_unk_71726bc570->field_0xb8 ? "#GUI_BOSSRUSH_PROMPT_QUIT_BOSSRUSH" : "#GUI_OPTIONS_PROMPT_QUIT",
                true);
    UnkButtonCallback_ovb0136 accept_callback = {self, fn_7100b1ef00};
    UnkButtonCallback_ovb0136 back_callback = {self, fn_7100b1ef58};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

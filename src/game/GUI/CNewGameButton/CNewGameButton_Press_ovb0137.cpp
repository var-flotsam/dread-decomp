// GUI::CNewGameButton's unit (tools/tu_map.py): the two new-game confirmations put up on the main menu's pop-up composition.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

UnkReflObject* fn_7100e033a8(UnkGameManager* manager);

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

struct UnkNewGameButton_ovb0137;
void fn_7100b5e1fc(UnkNewGameButton_ovb0137* self);
void fn_7100b5e200(UnkNewGameButton_ovb0137* self);
void fn_7100b5e204(UnkNewGameButton_ovb0137* self);

// A callback: an object and a function to call on it.
struct UnkNewGameCallback_ovb0137 {
    UnkNewGameButton_ovb0137* object;
    void (*function)(UnkNewGameButton_ovb0137*);
};

struct UnkMainMenu_ovb0137 {
    u8 pad_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

struct UnkPopUp_ovb0137 {
    u8 pad_0x0[0x3a0];
    bool busy;     // 0x3a0
    bool asking;   // 0x3a1
};

struct UnkNewGameButton_ovb0137 {
    u8 pad_0x0[0x2a1];
    bool field_0x2a1;
};

// The button's press: when the main menu has its pop-up composition, marks it busy and puts the
// "#GUI_FILEMENU_PROMPT_NEWGAME_CONFIRM_2" question up with accept and back texts and the button's two
// callbacks (fn_7100b5e204, fn_7100b5e200), as fn_7100afa114
// (src/game/GUI/CContinueGameButton/CContinueGameButton_Press_ovb0134.cpp).
void fn_7100b5db6c(UnkNewGameButton_ovb0137* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (!menu || !fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return;
    UnkReflObject* popup = reinterpret_cast<UnkMainMenu_ovb0137*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    reinterpret_cast<UnkPopUp_ovb0137*>(popup)->busy = true;
    CStrId text("#GUI_FILEMENU_PROMPT_NEWGAME_CONFIRM_2", true);
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    UnkNewGameCallback_ovb0137 accept_callback = {self, fn_7100b5e204};
    UnkNewGameCallback_ovb0137 back_callback = {self, fn_7100b5e200};
    fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

// The first confirmation: the main menu's display switched off (fn_71011cf2a4), the button's flag at
// +0x2a1 cleared, and "#GUI_FILEMENU_PROMPT_NEWGAME_CONFIRM_1" put up with the callbacks fn_7100b5e1fc
// and fn_7100b5e200.
void fn_7100b5d9a0(UnkNewGameButton_ovb0137* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (!menu || !fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return;
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), false);
    self->field_0x2a1 = false;
    UnkReflObject* popup = reinterpret_cast<UnkMainMenu_ovb0137*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    reinterpret_cast<UnkPopUp_ovb0137*>(popup)->asking = true;
    CStrId text("#GUI_FILEMENU_PROMPT_NEWGAME_CONFIRM_1", true);
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    UnkNewGameCallback_ovb0137 accept_callback = {self, fn_7100b5e1fc};
    UnkNewGameCallback_ovb0137 back_callback = {self, fn_7100b5e200};
    fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

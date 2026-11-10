// GUI::CContinueGameButton's unit (tools/tu_map.py unit 1657): fn_7100afa114 is the button's press.
// When the button asks first (+0x2a1) it puts the "#GUI_OPTIONS_PROMPT_LOAD_SAVEGAME" question up
// through the GUI composition (the game manager's +0x2258, its +0x10), with accept and back texts and
// the button's two callbacks (fn_7100afacd0, fn_7100afacd4), like fn_7100a86d00
// (src/game/CMapAcquisitionComponent/CMapAcquisitionComponent_Question_ovb0090.cpp); otherwise it
// continues the game at once (fn_7100afa28c). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkContinueGameButton_ovb0134 {
    u8 pad_0x0[0x2a1];
    bool ask_first;  // 0x2a1
};

void fn_7100afacd0(UnkContinueGameButton_ovb0134* self);
void fn_7100afacd4(UnkContinueGameButton_ovb0134* self);
void fn_7100afa28c(UnkContinueGameButton_ovb0134* self);

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

// A callback: an object and a function to call on it.
struct UnkContinueCallback_ovb0134 {
    UnkContinueGameButton_ovb0134* object;
    void (*function)(UnkContinueGameButton_ovb0134*);
};

struct UnkGuiRef_ovb0134 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

void fn_7100afa114(UnkContinueGameButton_ovb0134* self) {
    if (!self->ask_first)
        return fn_7100afa28c(self);
    UnkGuiRef_ovb0134* ref = static_cast<UnkGuiRef_ovb0134*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId text("#GUI_OPTIONS_PROMPT_LOAD_SAVEGAME", true);
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    UnkContinueCallback_ovb0134 accept_callback = {self, fn_7100afacd0};
    UnkContinueCallback_ovb0134 back_callback = {self, fn_7100afacd4};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

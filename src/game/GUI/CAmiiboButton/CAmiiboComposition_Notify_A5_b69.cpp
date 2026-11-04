// GUI::CAmiiboComposition: builds a GUI event of kind 0x12 for itself and hands it to the in-game
// menu's vf_0xb8 (when there is one). The layout is a placeholder.
#include "game/UnkGameManager.h"
#include "game/UnkGuiEvent.h"

void fn_7100af1e1c(UnkGuiDisplayObject* self) {
    UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310);
    if (menu) {
        UnkGuiEvent event;
        s32 kind = 0x12;
        fn_710120235c(&event, self, &kind, nullptr);
        (*reinterpret_cast<void (***)(UnkIngameMenu*, UnkGuiEvent*)>(menu))[0xb8 / 8](menu, &event);
        fn_7101202488(&event);
    }
}

// GUI::CMissionLog: builds a GUI event of kind 0x11 for itself and hands it to its own vf_0xb8.
// The layout is a placeholder.
#include "game/UnkGuiEvent.h"

void fn_7100b5cec0(UnkGuiDisplayObject* self) {
    s32 kind = 0x11;
    UnkGuiEvent event;
    fn_710120235c(&event, self, &kind, nullptr);
    (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, UnkGuiEvent*)>(self))[0xb8 / 8](self, &event);
    fn_7101202488(&event);
}

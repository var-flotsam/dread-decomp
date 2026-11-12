// Unit 1695 of tools/tu_map.py (no name hint). fn_7100b4f074: the first call of a menu screen's set-up
// looks up the "EndingRewardsGalleryComposition" composition through the GUI registry and keeps the
// object at +0x8 (as src/game_aca57c/fn_7100b4d0ec.cpp does for its compositions): it is hidden, unfocused
// and given its value, vf_0x78(-1500, true) and 2, then handed to the main menu (when the game manager's
// current menu is a GUI::CMainMenu-like class, else to nothing). Always true. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflObject;

struct UnkGalleryScreen_b257i {
    u8 pad_0x0[8];
    UnkGuiDisplayObject* composition;  // 0x8
};

// vf_0x78 of a composition.
static inline void CallVf78(void* composition, s32 a, bool b) {
    void** vtable = *static_cast<void***>(composition);
    reinterpret_cast<void (*)(void*, s32, bool)>(vtable[0x78 / 8])(composition, a, b);
}

bool fn_7100b4f074(UnkGalleryScreen_b257i* self) {
    if (self->composition)
        return true;
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString("EndingRewardsGalleryComposition")).Get();
    fn_71011cdce8(self->composition, 0);
    fn_71011cf2a4(self->composition, 0);
    fn_71011cf364(self->composition, 0);
    CallVf78(self->composition, -1500, true);
    fn_71011cde44(self->composition, 2);
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    UnkGuiDisplayObject* container = nullptr;
    if (menu)
        container = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? reinterpret_cast<UnkGuiDisplayObject*>(menu) : nullptr;
    fn_71011d968c(container, self->composition);
    return true;
}

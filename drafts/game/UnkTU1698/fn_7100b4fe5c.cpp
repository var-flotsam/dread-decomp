// Unit 1698 (tools/tu_map.py, no name hint): a menu screen's set-up. Looks the "MainMenuComposition"
// composition up through the GUI registry and keeps it at +0x8 (false when there is none), hides and
// unfocuses it and gives it its values, finds its three right-hand header parts by id, and hands it to the
// main menu (when the game manager's current menu is a GUI::CMainMenu, else to nothing). Names and layouts
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
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

struct UnkMainMenuScreen_lb4fe5c {
    u8 pad_0x0[8];
    UnkGuiDisplayObject* composition;  // 0x8
    u8 pad_0x10[0x20];
    UnkGuiDisplayObject* header_bg;     // 0x30
    UnkGuiDisplayObject* header_icon;   // 0x38
    UnkGuiDisplayObject* header_label;  // 0x40

    // Hidden, unfocused, and given its two values.
    void ResetComposition() {
        fn_71011cdce8(composition, false);
        fn_71011cf2a4(composition, false);
        fn_71011cf364(composition, 1);
        fn_71011cde44(composition, 2);
    }
};

// The id of a child's name: the CRC64 of the literal.
template <u64 N>
static inline u64 ChildId(const char (&name)[N]) {
    return StringCrc64(name, N - 1);
}

bool fn_7100b4fe5c(UnkMainMenuScreen_lb4fe5c* self) {
    self->composition = fn_71011b69d8(g_unk_717271e8f8, CRntString("MainMenuComposition")).Get();
    if (self->composition) {
        self->ResetComposition();
        self->header_bg = fn_71011d98ec(self->composition, ChildId("Header-R-Bg"));
        self->header_icon = fn_71011d98ec(self->composition, ChildId("Header-R-Icon"));
        self->header_label = fn_71011d98ec(self->composition, ChildId("Header-R-Label"));
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        UnkGuiDisplayObject* container = nullptr;
        if (menu)
            container = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? reinterpret_cast<UnkGuiDisplayObject*>(menu) : nullptr;
        fn_71011d968c(container, self->composition);
        return true;
    }
    return false;
}

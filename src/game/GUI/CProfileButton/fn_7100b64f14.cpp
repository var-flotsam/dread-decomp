// GUI::CProfileButton's unit (tools/tu_map.py unit 1712): the button's press on the main menu. A used
// profile slot is loaded (fn_7100df78cc) when the button has a target (+0x2a4 not negative), then the
// button's state step runs (fn_7100af65b4); an empty slot is created and the main menu goes to its page 8.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CChangeMenuStateButton.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflObject;

// As src/game/UnkTU1696/UnkTU1696_Refresh_lb4f408.cpp declares it: the profile's name for a slot.
// As src/game/UnkTU2666/UnkTU2666_lua_ovb0157.cpp declares it.
void fn_7100df96c0(UnkGameManager* self, const CRntString& name);
// As src/game/GUI/CAmiiboButton/CAmiiboButton_Press_laf05d0.cpp declares them.
struct UnkObj_7100af65b4;

// A profile slot is four 0x50-byte records; the first byte of the second says whether the slot is used.
struct UnkProfileRecord_lb64f14 {
    bool used;
    u8 pad_0x1[0x4f];
};

struct UnkMainMenu_lb64f14 {
    u8 pad_0x0[0xb0];
    UnkProfileRecord_lb64f14 records[3 * 4];  // 0xb0

    // Slots past the third fall back to the first.
    const UnkProfileRecord_lb64f14& Entry(s8 slot) {
        u32 index = slot;
        return records[index < 3 ? index * 4ull + 1 : 1];
    }
};

struct UnkProfileButton_lb64f14 {
    u8 pad_0x0[0x2a4];
    s32 target;  // 0x2a4
    s32 slot;    // 0x2a8
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100b64f14(UnkProfileButton_lb64f14* self) {
    auto* menu = static_cast<UnkMainMenu_lb64f14*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    if (!menu)
        return;
    s32 slot = self->slot;
    const UnkProfileRecord_lb64f14& entry = menu->Entry(slot);
    CRntString name = fn_7100df703c(slot);
    if (entry.used) {
        if (self->target >= 0) {
            fn_7100df78cc(g_unk_7101d47310, name, false, false);
            CChangeMenuStateButton_SendNewStateIdxMessage(reinterpret_cast<UnkObj_7100af65b4*>(self));
        }
    } else {
        fn_7100df97b4(g_unk_7101d47310, name, true, false);
        fn_7100df96c0(g_unk_7101d47310, name);
        UnkReflObject* main = fn_7100e033a8(g_unk_7101d47310);
        if (main && !fn_710009d50c(main->vf_0x0(), fn_7100b34230()))
            main = nullptr;
        fn_7100b35704(main, 8, true);
    }
}

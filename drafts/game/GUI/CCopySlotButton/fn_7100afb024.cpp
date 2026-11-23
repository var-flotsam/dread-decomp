// GUI::CCopySlotButton's unit (tools/tu_map.py unit 1658): the press on a target slot. The main menu's display
// is switched off; when the main menu's save screen shows the target slot (+0x2c4) as used, the pop-up
// asks "#GUI_FILESELECT_OPTIONS_PROMPT_OVWR_CONFIRM_<slot + 1>" with the button's two callbacks
// (fn_7100afb870 on accept, fn_7100afb874 on back); otherwise the copy goes ahead (fn_7100afb2b8). Names
// and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;

struct UnkCopySlotButton_ovb0131 {
    u8 pad_0x0[0x2c4];
    s32 target_slot;  // 0x2c4
};

void fn_7100afb2b8(UnkCopySlotButton_ovb0131* self);
void fn_7100afb870(UnkCopySlotButton_ovb0131* self);
void fn_7100afb874(UnkCopySlotButton_ovb0131* self);

// A callback: an object and a function to call on it.
struct UnkCopySlotCallback_ovb0131 {
    UnkCopySlotButton_ovb0131* object;
    void (*function)(UnkCopySlotButton_ovb0131*);
};

struct UnkCopySlotMenu_ovb0131 {
    u8 unk_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

struct UnkCopySlotPopUp_lafb024 {
    u8 pad_0x0[0x3a1];
    bool asking;  // 0x3a1
};

struct alignas(8) UnkEntryData_b0235 {
    u8 bytes[0x80];
    CRntString name;  // 0x80
};
void fn_7100afa4c0(UnkEntryData_b0235* self, const UnkEntryData_b0235* other);

// The save screen's entries (0x50 bytes each); each slot's data is in one of them.
struct UnkSaveEntry_lafb024 {
    u8 pad_0x0[0x50];
};

struct UnkSaveScreen_lafb024 {
    u8 pad_0x0[0xb0];
    UnkSaveEntry_lafb024 entries[13];  // 0xb0

    // The slot's data; the first slot's for an index out of range.
    const UnkEntryData_b0235* SlotData(s8 slot) {
        // Four entries a slot; the data is the second.
        u32 n = slot;
        u64 index = n < 3 ? (u64)n * 4 + 1 : 1;
        return reinterpret_cast<const UnkEntryData_b0235*>(&entries[index]);
    }
};

static inline UnkReflObject* MainMenu() {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (menu && fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return menu;
    return nullptr;
}

void fn_7100afb024(UnkCopySlotButton_ovb0131* self) {
    UnkReflObject* menu = MainMenu();
    if (menu)
        fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), false);
    s8 slot = self->target_slot;
    auto* screen = static_cast<UnkSaveScreen_lafb024*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    if (!screen) {
        fn_7100afb2b8(self);
        return;
    }
    bool used;
    {
        // A copy of the slot's data, built in place (fn_7100afa4c0 is its copy constructor).
        alignas(8) u8 data_storage[sizeof(UnkEntryData_b0235)];
        UnkEntryData_b0235* data = reinterpret_cast<UnkEntryData_b0235*>(data_storage);
        fn_7100afa4c0(data, screen->SlotData(slot));
        used = data->bytes[0];
        CRntString_Destroy(&data->name);
    }
    if (!used) {
        fn_7100afb2b8(self);
        return;
    }
    // The main menu is not checked again here.
    UnkReflObject* popup = reinterpret_cast<UnkCopySlotMenu_ovb0131*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    reinterpret_cast<UnkCopySlotPopUp_lafb024*>(popup)->asking = true;
    alignas(8) u8 prompt_storage[sizeof(CRntString)];
    CRntString* prompt = reinterpret_cast<CRntString*>(prompt_storage);
    fn_710007e6c4(prompt);
    CRntString_AppendCString(prompt, "#GUI_FILESELECT_OPTIONS_PROMPT_OVWR_CONFIRM_");
    fn_710007f634(prompt, self->target_slot + 1);
    {
        CStrId back("#GUI_B_BACK", true);
        CStrId accept("#GUI_A_ACCEPT", true);
        CStrId text(prompt->data ? prompt->data : g_unk_710162e2ed, true);
        UnkCopySlotCallback_ovb0131 accept_callback = {self, fn_7100afb870};
        UnkCopySlotCallback_ovb0131 back_callback = {self, fn_7100afb874};
        fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
    }
    CRntString_Destroy(prompt);
}

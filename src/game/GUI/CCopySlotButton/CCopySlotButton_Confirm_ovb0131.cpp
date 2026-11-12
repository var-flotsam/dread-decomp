// GUI::CCopySlotButton's unit (tools/tu_map.py unit 1658): asks the second copy confirmation. When there
// is a main menu whose pop-up (+0x540) is a GUI::CPopUpComposition (as fn_7100afb5f8), marks the pop-up
// (+0x3a0) and shows "#GUI_FILESELECT_OPTIONS_PROMPT_SECOND_CONFIRM" with accept and back texts and two
// callbacks on the button (fn_7100afb878 on accept, fn_7100afb874 on back) through fn_7100b5fde0 (as
// src/game/CMapAcquisitionComponent/CMapAcquisitionComponent_Question_ovb0090.cpp). Names and layouts
// are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;

struct UnkCopySlotButton_ovb0131;
void fn_7100afb874(UnkCopySlotButton_ovb0131* self);
void fn_7100afb878(UnkCopySlotButton_ovb0131* self);

// A callback: an object and a function to call on it.
struct UnkCopySlotCallback_ovb0131 {
    UnkCopySlotButton_ovb0131* object;
    void (*function)(UnkCopySlotButton_ovb0131*);
};

struct UnkCopySlotMenu_ovb0131 {
    u8 unk_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

struct UnkCopySlotPopUp_ovb0131 {
    u8 unk_0x0[0x3a0];
    bool asking;  // 0x3a0
};

void fn_7100afb410(UnkCopySlotButton_ovb0131* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (!menu || !fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return;
    UnkReflObject* popup = reinterpret_cast<UnkCopySlotMenu_ovb0131*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    reinterpret_cast<UnkCopySlotPopUp_ovb0131*>(popup)->asking = true;
    alignas(8) u8 prompt_storage[sizeof(CRntString)];
    CRntString* prompt = reinterpret_cast<CRntString*>(prompt_storage);
    fn_710007e6c4(prompt);
    fn_710007f564(prompt, "#GUI_FILESELECT_OPTIONS_PROMPT_SECOND_CONFIRM");
    {
        CStrId back("#GUI_B_BACK", true);
        CStrId accept("#GUI_A_ACCEPT", true);
        CStrId text(prompt->data ? prompt->data : g_unk_710162e2ed, true);
        UnkCopySlotCallback_ovb0131 accept_callback = {self, fn_7100afb878};
        UnkCopySlotCallback_ovb0131 back_callback = {self, fn_7100afb874};
        fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
    }
    fn_710007ecd4(prompt);
}

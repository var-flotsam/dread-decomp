// GUI::CDeleteSlotButton's unit (tools/tu_map.py unit 1664): asks to confirm deleting the button's slot.
// When there is a main menu whose pop-up (+0x540) is a GUI::CPopUpComposition, switches the menu's display
// off (fn_71011cf2a4), marks the pop-up (+0x3a1) and shows "#GUI_FILESELECT_OPTIONS_PROMPT_DELETE_CONFIRM_"
// with the slot's number, accept and back texts and the button's two callbacks (fn_7100b01090 on accept,
// fn_7100b01094 on back), as fn_7100afb410 does for copying. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
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

struct UnkDeleteSlotButton {
    char* vtable;
    u8 base_0x8[0x2a0];
    void* field_0x2a8;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    s32 slot;  // 0x2c0
};
static_assert(__builtin_offsetof(UnkDeleteSlotButton, slot) == 0x2c0, "UnkDeleteSlotButton layout");

void fn_7100b01090(UnkDeleteSlotButton* self);
void fn_7100b01094(UnkDeleteSlotButton* self);

// A callback: an object and a function to call on it.
struct UnkDeleteSlotCallback_lb00970 {
    UnkDeleteSlotButton* object;
    void (*function)(UnkDeleteSlotButton*);
};

struct UnkDeleteSlotMenu_lb00970 {
    u8 unk_0x0[0x540];
    UnkReflObject* popup;  // 0x540
};

struct UnkDeleteSlotPopUp_lb00970 {
    u8 unk_0x0[0x3a1];
    bool asking;  // 0x3a1
};

void fn_7100b00970(UnkDeleteSlotButton* self) {
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (!menu || !fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
        return;
    fn_71011cf2a4(reinterpret_cast<UnkGuiDisplayObject*>(menu), false);
    UnkReflObject* popup = reinterpret_cast<UnkDeleteSlotMenu_lb00970*>(menu)->popup;
    if (!popup || !fn_710009d50c(popup->vf_0x0(), fn_7100b5e368()))
        return;
    reinterpret_cast<UnkDeleteSlotPopUp_lb00970*>(popup)->asking = true;
    alignas(8) u8 prompt_storage[sizeof(CRntString)];
    CRntString* prompt = reinterpret_cast<CRntString*>(prompt_storage);
    fn_710007e6c4(prompt);
    fn_710007f564(prompt, "#GUI_FILESELECT_OPTIONS_PROMPT_DELETE_CONFIRM_");
    fn_710007f634(prompt, self->slot + 1);
    {
        CStrId back("#GUI_B_BACK", true);
        CStrId accept("#GUI_A_ACCEPT", true);
        CStrId text(prompt->data ? prompt->data : g_unk_710162e2ed, true);
        UnkDeleteSlotCallback_lb00970 accept_callback = {self, fn_7100b01090};
        UnkDeleteSlotCallback_lb00970 back_callback = {self, fn_7100b01094};
        fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(popup), &text, &accept, &back,
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                      reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
    }
    fn_710007ecd4(prompt);
}

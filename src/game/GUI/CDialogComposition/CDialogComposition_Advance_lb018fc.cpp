// GUI::CDialogComposition's unit (tools/tu_map.py unit 1665): the dialog's input handler. Unless the event
// was handled, the dialog is inactive, or (for a confirm, kind 0x12) the in-game or main menu is busy, it
// either lets the progress label (g_unk_717275db70) take the event or, at the label's end, closes the page
// and fires the confirm (0x12) or cancel (0x11) script function and callback. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDialogComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkDlgIdData_ovb0069.h"
#include "game/UnkDlgId_ovb0069.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

using UnkGuiObj = UnkGuiDisplayObject;

static inline UnkGuiObj* FindLabel(void* self, const UnkDlgId_ovb0069& id) {
    UnkGuiObj* obj;
    {
        CRntString path(id.Text());
        obj = fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
        if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                  reinterpret_cast<const base::reflection::CType*>(fn_71011e8dfc())))
            obj = nullptr;
    }
    return obj;
}

// The label's current and last positions.
s32 fn_71011ef54c(UnkGuiDisplayObject* self);
s32 fn_71011ef544(UnkGuiDisplayObject* self);

struct UnkDialogClose_ovb0069;
void fn_7100b02404(UnkDialogClose_ovb0069* self);

// The game's mode holder (+0x498 of the game manager): its name at +0x8.
struct UnkDlgModeHolder_lb018fc {
    u8 pad_0x0[0x8];
    CRntString name;
};

struct UnkDlgIngameMenu_lb018fc {
    u8 pad_0x0[0x2e0];
    s32 state;
};

struct UnkDlgMainMenu_lb018fc {
    u8 pad_0x0[0x278];
    s32 state;
};

struct UnkDlgEvent_lb018fc {
    u8 pad_0x0[0x8];
    s32 kind;
    u8 pad_0xc[0xe];
    bool handled;  // 0x1a
};

struct UnkDlgOwner_lb018fc {
    u8 pad_0x0[0x11];
    bool active;
};

// A callback bound to its argument.
struct UnkDlgAction_lb018fc {
    void* arg;
    u32 (*fn)(void* arg);
};

struct UnkDialog_lb018fc {
    u8 pad_0x0[0x30];
    UnkDlgOwner_lb018fc* owner;  // 0x30
    u8 pad_0x38[0x7c];
    bool active;  // 0xb4
    u8 pad_0xb5[0x233];
    CStrId cancel_function;  // 0x2e8
    UnkDlgAction_lb018fc on_cancel;  // 0x2f0
    CStrId confirm_function;  // 0x300
    UnkDlgAction_lb018fc on_confirm;  // 0x308
};
static_assert(__builtin_offsetof(UnkDialog_lb018fc, cancel_function) == 0x2e8, "layout");

// Whether the menu of the current mode (in game or main menu) is idle (state 9).
static inline bool MenuIdle() {
    UnkDlgModeHolder_lb018fc* mode = static_cast<UnkDlgModeHolder_lb018fc*>(g_unk_7101d47310->field_0x498);
    if (fn_710007f140(&mode->name, "INGAME") &&
        reinterpret_cast<UnkDlgIngameMenu_lb018fc*>(fn_7100deac84(g_unk_7101d47310))->state != 9)
        return false;
    mode = static_cast<UnkDlgModeHolder_lb018fc*>(g_unk_7101d47310->field_0x498);
    if (fn_710007f140(&mode->name, "MAINMENU")) {
        base::reflection::UnkReflObject* obj = fn_7100e033a8(g_unk_7101d47310);
        auto* menu = fn_710009d50c(obj->vf_0x0(), fn_7100b34230()) ? reinterpret_cast<UnkDlgMainMenu_lb018fc*>(obj)
                                                                  : nullptr;
        if (menu->state != 9)
            return false;
    }
    return true;
}

// Calls the script function named by `function`, resets it to the default id and runs the callback.
static inline void Fire(CStrId& function, UnkDlgAction_lb018fc& action) {
    fn_7100000790(function.Text(), g_unk_710162e2ed);
    function = base::global::CStrId::Empty();
    if (action.fn)
        action.fn(action.arg);
}

void fn_7100b018fc(UnkMissionLogTarget_ovb0079* target, UnkGuiEvent* gui_event) {
    UnkDialog_lb018fc* self = reinterpret_cast<UnkDialog_lb018fc*>(target);
    UnkDlgEvent_lb018fc* event = reinterpret_cast<UnkDlgEvent_lb018fc*>(gui_event);
    if (event && event->handled)
        return;
    if (!self->active || !self->owner->active)
        return;
    if (event->kind != 0x12 || MenuIdle()) {
        UnkGuiObj* label = FindLabel(self, g_unk_717275db70);
        if (fn_71011ef54c(label) < fn_71011ef544(label)) {
            reinterpret_cast<void (*)(UnkGuiObj*, UnkDlgEvent_lb018fc*)>(
                (*reinterpret_cast<void***>(label))[0xf0 / 8])(label, event);
        } else {
            fn_7100b02404(reinterpret_cast<UnkDialogClose_ovb0069*>(self));
            if (event->kind == 0x11)
                Fire(self->cancel_function, self->on_cancel);
            else if (event->kind == 0x12)
                Fire(self->confirm_function, self->on_confirm);
        }
    }
    event->handled = true;
}

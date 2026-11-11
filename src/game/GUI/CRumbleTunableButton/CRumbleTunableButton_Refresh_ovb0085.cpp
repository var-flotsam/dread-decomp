// GUI::CRumbleTunableButton's unit (tools/tu_map.py): refreshing the button (after its base step
// fn_71011bfd28): whether rumble is enabled, from the rumble manager (created on first use), kept at
// +0x2a1, the button's label (fn_71011c08dc) set to the matching "on"/"off" text, and the label
// refreshed. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/input/UnkRumbleManager.h"
#include "game/GUI/CButton.h"
#include "types.h"

using base::global::CRntString;
using base::input::UnkRumbleManager;

struct UnkGuiObj;
// As src/game/GUI/CMissionLog/CMissionLog_Lifecycle_ovb0079.cpp declares it: the button's label.
struct UnkMissionLogButton_ovb0079;
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011cd5a0(UnkGuiObj*);
extern UnkRumbleManager* g_unk_7101d47348;
bool fn_710007453c(UnkRumbleManager* self);

struct UnkRumbleTunableButton_ovb0085 {
    u8 unk_0x0[0x2a1];
    bool enabled;  // 0x2a1
};

static inline UnkGuiObj* Label(UnkRumbleTunableButton_ovb0085* self) {
    return fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self));
}

void fn_7100b65b94(UnkRumbleTunableButton_ovb0085* self) {
    fn_71011bfd28(self);
    UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    bool enabled = fn_710007453c(manager);
    self->enabled = enabled;
    UnkGuiObj* label = Label(self);
    if (enabled)
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_ON"));
    else
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_OFF"));
    fn_71011cd5a0(Label(self));
}

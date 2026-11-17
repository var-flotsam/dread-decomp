// GUI::CRumbleTunableButton's unit (tools/tu_map.py): setting the button up (after its base step
// fn_71011bf3a0): whether rumble is enabled, from the rumble manager, then from the "RumbleTunable" setting
// the user data blackboard saves (applied back to the manager), kept at +0x2a1; the button's label set to
// the matching "on"/"off" text and refreshed. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "game/CBlackboard.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CLabel.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2768.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::input::UnkRumbleManager;

using UnkGuiObj = UnkGuiDisplayObject;
// As src/game/GUI/CMissionLog/CMissionLog_Lifecycle_ovb0079.cpp declares it: the button's label.
struct UnkMissionLogButton_ovb0079;

struct UnkRumbleTunableButton_ovb0085 {
    u8 unk_0x0[0x2a1];
    bool enabled;  // 0x2a1
};

// The button's base setup.
void fn_71011bf3a0(UnkRumbleTunableButton_ovb0085* self);

static inline UnkGuiObj* Label(UnkRumbleTunableButton_ovb0085* self) {
    return fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self));
}

static inline UnkRumbleManager* RumbleManager() {
    UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    return manager;
}

// Whether rumble is enabled: the second byte of the manager's flags at +0x2208.
static inline bool IsRumbleEnabled(UnkRumbleManager* manager) {
    return (manager->field_0x2208 >> 8) != 0;
}

void fn_7100b654d0(UnkRumbleTunableButton_ovb0085* self) {
    fn_71011bf3a0(self);
    self->enabled = IsRumbleEnabled(RumbleManager());
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    CBlackboard* blackboard = fn_710120384c(owner);
    bool known;
    {
        CStrId section("USER_DATA", true);
        CStrId name("RumbleTunable", true);
        known = fn_71010474f4(blackboard, section, name);
    }
    if (known) {
        {
            CStrId section("USER_DATA", true);
            CStrId name("RumbleTunable", true);
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                fn_710006d11c(s, &name, &self->enabled);
        }
        fn_7100074e3c(RumbleManager(), self->enabled);
    }
    bool enabled = self->enabled;
    UnkGuiObj* label = Label(self);
    if (enabled)
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_ON"));
    else
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_OFF"));
    fn_71011cd5a0(Label(self));
}

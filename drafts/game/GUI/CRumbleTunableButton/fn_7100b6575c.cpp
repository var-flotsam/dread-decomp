// GUI::CRumbleTunableButton's unit (tools/tu_map.py): pressing the button. It updates the button's list
// (+0x120, when it is a GUI::CList), flips whether rumble is enabled (+0x2a1) and its "on"/"off" label,
// saves the "RumbleTunable" setting to the user data blackboard, applies it to the rumble manager, plays
// the press sound and, when rumble is now on, a short sample rumble. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CLabel.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2768.h"
#include "game/UnkTypedValue.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::input::UnkRumbleManager;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

using UnkGuiObj = UnkGuiDisplayObject;
// As src/game/GUI/CMissionLog/CMissionLog_Lifecycle_ovb0079.cpp declares it: the button's label.
struct UnkMissionLogButton_ovb0079;

struct UnkRumbleTunableButton_lb6575c {
    u8 pad_0x0[0x120];
    UnkReflObject* list;  // 0x120
    u8 pad_0x128[0x179];
    bool enabled;  // 0x2a1
};
static_assert(__builtin_offsetof(UnkRumbleTunableButton_lb6575c, enabled) == 0x2a1, "layout");

// Updates a GUI::CList.
void fn_710120186c(UnkReflObject* list);

// The press sound's event.
extern UnkStateKey g_unk_71729f02a0;

// What a rumble is played from (the UnkRumbleParams of src/game/CRumbleComponent.cpp).
struct UnkRumbleParams;
struct UnkRumbleTunableParams_lb6575c {
    CFilePathStrId path;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleTunableParams_lb6575c()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};
static_assert(sizeof(UnkRumbleTunableParams_lb6575c) == 0x48, "layout");

struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};
UnkHandle fn_7100074354(UnkRumbleManager* self, const UnkRumbleParams* params);

static inline UnkGuiObj* Label(UnkRumbleTunableButton_lb6575c* self) {
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

static inline UnkUserDataOwner* UserDataOwner() {
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return owner;
}

void fn_7100b6575c(UnkRumbleTunableButton_lb6575c* self) {
    UnkReflObject* list = self->list;
    fn_710120186c(list && fn_710009d50c(list->vf_0x0(), fn_71011fe904()) ? list : nullptr);

    bool enabled = !self->enabled;
    self->enabled = enabled;
    UnkGuiObj* label = Label(self);
    if (enabled)
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_ON"));
    else
        fn_71011ec424(label, CRntString("#GUI_OPTIONS_LABEL_RUMBLE_OFF"));
    fn_71011cd5a0(Label(self));

    CBlackboard* blackboard = fn_710120384c(UserDataOwner());
    {
        CStrId section("USER_DATA", true);
        CStrId name("RumbleTunable", true);
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
            if (!blackboard->field_0xe8) {
                UnkTypedValue v;
                v.type = fn_710000d228();
                v.value = &self->enabled;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
            }
        }
    }
    fn_7101203854(UserDataOwner());
    fn_7100074e3c(RumbleManager(), self->enabled);
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f02a0, self);

    if (self->enabled) {
        static CFilePathStrId s_path("generic/rumble_on.bnvib", true);
        static UnkRumbleTunableParams_lb6575c s_params;
        s_params.path = s_path;
        s_params.field_0x10 = 1;
        s_params.field_0x20[2] = 1.0f;
        s_params.field_0x34 = 0.1f;
        fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&s_params));
    }
}

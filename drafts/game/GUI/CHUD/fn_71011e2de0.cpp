// GUI::CHUD (unit 2751): on an Aeion change, fills the HUD's Aeion gauges to the new share of the
// maximum and fires the gauge's "OnAeionDepletion", "OnAeionFill", "OnAeionObtained" or
// "OnAeionRecharge" event. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CTutoEnterLogicAction.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using UnkGuiObj = UnkGuiDisplayObject;
using UnkVec2f = base::math::CVector2D;

struct UnkEnumType;
struct UnkPopUpComposition_ovb0085;

void fn_71011cfdb8(UnkGuiObj* object, const UnkVec2f* scale);
void fn_7100b6026c(UnkPopUpComposition_ovb0085* self, bool value);
extern UnkEnumType g_unk_71729f0280;  // the Aeion obtained sound's event
extern UnkEnumType g_unk_71729f0288;  // the Aeion fill sound's event

// A HUD item: its event slot (vtable 0x1e8) plays the named animation event.
struct UnkHudItem_2de0 {
    void** vtable;
    void SendEvent(const char* event, bool flag) {
        reinterpret_cast<void (*)(UnkHudItem_2de0*, const char*, bool)>(vtable[0x1e8 / 8])(this, event, flag);
    }
};

// A gauge: its fill at +0x94.
struct UnkHudGauge_2de0 {
    u8 pad_0x0[0x94];
    f32 fill;  // 0x94
};

template <typename T>
struct UnkHudHandle_2de0 {
    u8 pad_0x0[0x10];
    T* target;  // 0x10
};

struct UnkHud_2de0 {
    u8 pad_0x0[0x234];
    bool sounds;  // 0x234
    u8 pad_0x235;
    bool hidden;  // 0x236
    u8 pad_0x237[0x21];
    bool obtained;  // 0x258
    u8 pad_0x259[0x7f];
    UnkHudHandle_2de0<UnkHudItem_2de0>* aeion;  // 0x2d8
    UnkHudHandle_2de0<UnkHudGauge_2de0>* gauge;  // 0x2e0
    UnkHudHandle_2de0<UnkHudGauge_2de0>* gauge_2;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkHud_2de0, aeion) == 0x2d8, "UnkHud_2de0 layout");

// The change: the Aeion and its maximum, and whether it comes without its sound.
struct UnkAeionChange_2de0 {
    f32 value;
    f32 max;
    bool silent;
};

struct UnkPlayerState_2de0 {
    u8 pad_0x0[0x70];
    u32 state;  // 0x70
};
struct UnkPlayerView_2de0 {
    u8 pad_0x0[0x1b8];
    UnkPlayerState_2de0* field_0x1b8;
};

// The pop-up composition: the set of its listeners at +0x260.
struct UnkPopUp_2de0 {
    u8 pad_0x0[0x260];
    u8 listeners[0x8];  // 0x260
};
struct UnkIngameMenu_2de0 {
    u8 pad_0x0[0x230];
    void* popup_listener;  // 0x230
};

static inline UnkPopUp_2de0* PopUp() {
    auto* handle = static_cast<UnkHudHandle_2de0<UnkPopUp_2de0>*>(g_unk_7101d47310->field_0x2258);
    return handle ? handle->target : nullptr;
}

// Moves the listener in `slot` to the pop-up's set of listeners.
static inline void Listen(void** slot, UnkPopUp_2de0* popup) {
    if (*slot) {
        fn_71004567dc(*slot, slot);
        *slot = nullptr;
    }
    *slot = popup->listeners;
    fn_7100a58e80(popup->listeners, slot);
}

bool fn_71011e2de0(UnkHud_2de0* self, UnkAeionChange_2de0 change) {
    auto* player = reinterpret_cast<UnkPlayerView_2de0*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return true;
    if (player->field_0x1b8 && player->field_0x1b8->state == 2)
        return true;
    if (self->hidden)
        return false;
    if (!self->aeion || !self->aeion->target)
        return true;
    UnkHudItem_2de0* bar = self->aeion->target;
    fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(bar), true);
    if (!self->gauge || !self->gauge->target)
        return false;
    UnkHudGauge_2de0* gauge = self->gauge->target;
    f32 old = gauge->fill;
    f32 share = change.value / change.max;
    if (!(__builtin_fabsf(old - share) > 1.1920929e-7f))
        return false;
    UnkVec2f scale = {share, 1.0f};
    fn_71011cfdb8(reinterpret_cast<UnkGuiDisplayObject*>(gauge), &scale);
    if (self->gauge_2 && self->gauge_2->target) {
        scale = {share, 1.0f};
        fn_71011cfdb8(reinterpret_cast<UnkGuiDisplayObject*>(self->gauge_2->target), &scale);
    }

    if (share < 1.1920929e-7f) {
        bar->SendEvent("OnAeionDepletion", false);
    } else if (share > 0.99999988f) {
        bar->SendEvent("OnAeionFill", false);
        if (!change.silent && self->sounds)
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0288), self);
        if (self->obtained) {
            self->obtained = false;
            auto* menu = reinterpret_cast<UnkIngameMenu_2de0*>(fn_7100deac84(g_unk_7101d47310));
            Listen(&menu->popup_listener, PopUp());
            fn_7100b6026c(reinterpret_cast<UnkPopUpComposition_ovb0085*>(PopUp()), true);
        }
    } else if (share > old) {
        if (self->obtained) {
            if (!change.silent && self->sounds)
                fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0280), self);
        }
        if (self->obtained)
            bar->SendEvent("OnAeionObtained", false);
        else
            bar->SendEvent("OnAeionRecharge", false);
    }
    return false;
}

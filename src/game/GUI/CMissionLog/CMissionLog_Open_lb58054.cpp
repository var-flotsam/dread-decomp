// GUI::CMissionLog (unit 1706): the log's event handler. On an open event (kind 2) it makes sure the
// dialog composition exists (found under the root's menu, or loaded and set up), shows or hides the three
// buttons at +0x2d8..+0x2e8 for the current input, resets the objectives list and the entries label,
// focuses the objectives list and works out the page step; then hands the event to the base handler.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/GUI/CList.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkList_ovb0059;
struct UnkSprite_spr;

extern __attribute__((visibility("hidden"))) CStrId g_unk_7172765050;  // the dialog composition's name
extern __attribute__((visibility("hidden"))) CStrId g_unk_7172765058;  // the entries label's font

void fn_7100e02e98(UnkGameManager* manager, UnkGuiDisplayObject* object);
void fn_7100b017fc(UnkGuiDisplayObject* dialog);

// The event the handler gets.
struct UnkMissionLogEvent_lb58054 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};

// A reference to a display object of the log's layout.
struct UnkMissionLogRef_lb58054 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
};

// The objectives list.
struct UnkMissionLogObjectives_lb58054 : UnkGuiDisplayObject {
    u8 pad_0x198[0x1c8 - 0x198];
    s32 count;  // 0x1c8
    u8 pad_0x1cc[0x230 - 0x1cc];
    u64 field_0x230;
    u32 field_0x238;
    u8 pad_0x23c[0x264 - 0x23c];
    bool field_0x264;

    void vf_0x230() { reinterpret_cast<void (*)(UnkMissionLogObjectives_lb58054*)>((*reinterpret_cast<void***>(this))[0x230 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkMissionLogObjectives_lb58054, field_0x264) == 0x264, "layout");

// The page counter.
struct UnkMissionLogCounter_lb58054 {
    u8 pad_0x0[0x8c];
    f32 length;  // 0x8c
};

struct UnkMissionLog_lb58054 : UnkGuiDisplayObject {
    u8 pad_0x198[0x238 - 0x198];
    UnkMissionLogObjectives_lb58054* objectives;  // 0x238
    UnkGuiDisplayObject* entries;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    UnkGuiDisplayObject* dialog;  // 0x298
    u8 pad_0x2a0[0x2b8 - 0x2a0];
    UnkMissionLogCounter_lb58054* page_counter;  // 0x2b8
    u8 pad_0x2c0[0x10];
    f32 page_step;  // 0x2d0
    u8 pad_0x2d4[0x4];
    UnkMissionLogRef_lb58054* buttons[3];  // 0x2d8

    void vf_0x1e8(const char* event, bool flag) {
        reinterpret_cast<void (*)(UnkMissionLog_lb58054*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, event, flag);
    }
};
static_assert(__builtin_offsetof(UnkMissionLog_lb58054, dialog) == 0x298, "layout");
static_assert(__builtin_offsetof(UnkMissionLog_lb58054, buttons) == 0x2d8, "layout");

static inline UnkGuiDisplayObject* Get(UnkMissionLogRef_lb58054* ref) {
    return ref ? ref->target : nullptr;
}

// `object` when it is a GUI::CDialogComposition, else null.
static inline UnkGuiDisplayObject* AsDialog(UnkGuiDisplayObject* object) {
    return object && fn_710009d50c(object->GetClass(), fn_7100b01160()) ? object : nullptr;
}

static inline void CallVf78(UnkGuiDisplayObject* object, s32 a, bool b) {
    void** vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<void (*)(UnkGuiDisplayObject*, s32, bool)>(vtable[0x78 / 8])(object, a, b);
}

static inline void ShowButtons(UnkMissionLog_lb58054* self, bool show) {
    fn_71011cdc30(Get(self->buttons[0]), show);
    fn_71011cdc30(Get(self->buttons[1]), show);
    fn_71011cdc30(Get(self->buttons[2]), show);
}

void fn_7100b58054(UnkMissionLog_lb58054* self, UnkMissionLogEvent_lb58054* event) {
    if (event->kind == 2) {
        if (!self->dialog) {
            UnkGuiDisplayObject* found = fn_71011d94d4(g_unk_717271e8f8->menu, g_unk_7172765050);
            if (AsDialog(found)) {
                self->dialog = found;
            } else {
                UnkGuiDisplayObject* dialog =
                    AsDialog(GetGuiCompositionHandleByName(g_unk_717271e8f8, CRntString("DialogComposition")).Get());
                if (dialog) {
                    fn_71011cdce8(dialog, false);
                    fn_71011cf2a4(dialog, false);
                    fn_71011cf364(dialog, -1);
                    CallVf78(dialog, -2000, true);
                    fn_71011cde44(dialog, 2);
                    fn_71011d968c(g_unk_717271e8f8->menu, dialog);
                    fn_7100e02e98(g_unk_7101d47310, dialog);
                    fn_7100b017fc(dialog);
                }
            }
        }
        if (fn_7100deac24(g_unk_7101d47310)) {
            ShowButtons(self, false);
        } else {
            fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
            ShowButtons(self, true);
        }
        fn_71011cd5a0(self);
        self->objectives->field_0x238 = 7;
        self->objectives->field_0x230 = 0;
        self->objectives->vf_0x230();
        fn_71012004ac(reinterpret_cast<UnkList_ovb0059*>(self->objectives));
        self->objectives->field_0x264 = true;
        fn_71011d0088(self->entries, false);
        CLabel_SetFont(self->entries, g_unk_7172765058);
        fn_71011ed018(self->entries, 0);
        fn_71011ed084(self->entries, 0);
        UnkSprite_spr* sprite = reinterpret_cast<UnkSprite_spr*>(self->entries);
        base::math::CVector2D size = {0.7f, 0.55f};
        fn_71011cf8d0(sprite, &size);
        fn_71011cdc30(self->objectives, true);
        fn_71011cdc30(self->entries, false);
        self->vf_0x1e8("EventsSelected", false);
        fn_71011cf2a4(self->objectives, true);
        fn_71011b6bbc(g_unk_717271e8f8, self->objectives, false);
        s32 last = self->objectives->count - 1;
        self->page_step = self->page_counter->length / last;
    }
    // The base handler takes the event too (declared elsewhere without it).
    reinterpret_cast<void (*)(UnkMissionLog_lb58054*, UnkMissionLogEvent_lb58054*)>(fn_71011d9ab4)(self, event);
}

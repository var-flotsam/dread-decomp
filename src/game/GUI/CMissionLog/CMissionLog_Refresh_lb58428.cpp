// GUI::CMissionLog (unit 1706): after the base update, while the log is shown, hides the page
// counter and both scroll arrows, then shows the counter and the arrows that apply for the
// current list and page, and labels the close button "close" or "back". Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CRntString;

// A reference to a display object of the log's layout.
struct UnkMissionLogRef_l58428 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
};

// The objectives list.
struct UnkMissionLogObjectives_l58428 {
    u8 pad_0x0[0xb5];
    bool field_0xb5;  // shown: the log pages through it
    u8 pad_0xb6[0x1c8 - 0xb6];
    s32 count;  // 0x1c8
    u8 pad_0x1cc[0x244 - 0x1cc];
    s32 page;  // 0x244
};

// The other list.
struct UnkMissionLogEntries_l58428 {
    u8 pad_0x0[0x18c];
    s32 count;  // 0x18c
    s32 page;  // 0x190
};

struct UnkMissionLog_l58428 : UnkGuiDisplayObject {
    u8 pad_0x198[0x238 - 0x198];
    UnkMissionLogObjectives_l58428* objectives;  // 0x238
    UnkMissionLogEntries_l58428* entries;  // 0x240
    u8 pad_0x248[0x2b8 - 0x248];
    UnkGuiDisplayObject* page_counter;  // 0x2b8
    u8 pad_0x2c0[8];
    UnkGuiDisplayObject* page_bar;  // 0x2c8
    f32 page_step;  // 0x2d0
    u8 pad_0x2d4[0x2f0 - 0x2d4];
    UnkMissionLogRef_l58428* close_label;  // 0x2f0
    UnkMissionLogRef_l58428* next_arrow;  // 0x2f8
    UnkMissionLogRef_l58428* prev_arrow;  // 0x300
};
static_assert(__builtin_offsetof(UnkMissionLog_l58428, objectives) == 0x238, "layout");
static_assert(__builtin_offsetof(UnkMissionLog_l58428, page_counter) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkMissionLog_l58428, prev_arrow) == 0x300, "layout");

static inline UnkGuiDisplayObject* Get(UnkMissionLogRef_l58428* ref) {
    return ref ? ref->target : nullptr;
}

// Display object vtable slot 0x1e8: sends an event by name.
static inline void SendEvent(UnkGuiDisplayObject* object, const char* name, bool flag) {
    reinterpret_cast<void (***)(UnkGuiDisplayObject*, const char*, bool)>(object)[0][0x1e8 / 8](
        object, name, flag);
}

static inline void ShowArrow(UnkMissionLogRef_l58428* const& arrow) {
    fn_71011cdc30(Get(arrow), true);
    SendEvent(Get(arrow), "ContinueArrowLoop", false);
}

void fn_7100b58428(UnkMissionLog_l58428* self) {
    fn_71011d9050(self);
    if (!self->field_0xb4)
        return;
    fn_71011cdc30(self->page_counter, false);
    fn_71011cdc30(Get(self->next_arrow), false);
    fn_71011cdc30(Get(self->prev_arrow), false);
    if (self->page_bar) {
        if (self->objectives->field_0xb5) {
            if (self->objectives->count >= 8) {
                fn_71011cdc30(self->page_counter, true);
                s32 page = self->objectives->page;
                fn_71011cca3c(self->page_bar, self->page_step * page);
                if (page >= 1)
                    ShowArrow(self->prev_arrow);
                if (page + 1 < self->objectives->count)
                    ShowArrow(self->next_arrow);
            }
        } else if (self->entries->count >= 1) {
            fn_71011cdc30(self->page_counter, true);
            s32 page = self->entries->page;
            fn_71011cca3c(self->page_bar, self->page_step * page);
            if (page >= 1)
                ShowArrow(self->prev_arrow);
            if (page < self->entries->count)
                ShowArrow(self->next_arrow);
        }
    }
    if (fn_7100deabc4(g_unk_7101d47310))
        fn_71011ec424(Get(self->close_label), CRntString("#GUI_MAPMENU_LABEL_CLOSE"));
    else if (fn_7100deac24(g_unk_7101d47310))
        fn_71011ec424(Get(self->close_label), CRntString("#GUI_MAPMENU_LABEL_BACK"));
}

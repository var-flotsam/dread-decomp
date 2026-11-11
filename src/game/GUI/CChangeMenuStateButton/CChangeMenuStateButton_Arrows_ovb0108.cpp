// GUI::CChangeMenuStateButton's unit (tools/tu_map.py): a paged screen's update. After the base
// update (fn_71011d8fe4), when the page changed (+0x248) and a page is selected, the next and
// previous arrows (+0x278, +0x280) are shown and loop their "ContinueArrowLoop" animation (vtable
// slot 0x1e8) unless on the last or first page, and the list (+0x270) scrolls to the page when there
// are more entries than fit. Names and layouts are placeholders.
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkNewGameScreen;
void fn_71011cca3c(UnkGuiDisplayObject* self, f32 value);

struct UnkPagedList_ovb0108 {
    u8 pad_0x0[0x1c8];
    s32 count;  // 0x1c8
    u8 pad_0x1cc[0x6c];
    s32 visible;  // 0x238
    u8 pad_0x23c[0x8];
    s32 page;  // 0x244
};

struct UnkPagedScreen_ovb0108 {
    u8 pad_0x0[0x248];
    bool changed;  // 0x248
    u8 pad_0x249[3];
    f32 page_step;  // 0x24c
    UnkPagedList_ovb0108* list;  // 0x250
    u8 pad_0x258[0x18];
    UnkGuiDisplayObject* scroll;  // 0x270
    UnkGuiDisplayObject* next;    // 0x278
    UnkGuiDisplayObject* prev;    // 0x280
};

static inline void PlayAnimation(UnkGuiDisplayObject* obj, const char* name) {
    reinterpret_cast<void (**)(UnkGuiDisplayObject*, const char*, bool)>(*reinterpret_cast<void***>(obj))[0x1e8 / 8](
        obj, name, false);
}

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100af587c(UnkPagedScreen_ovb0108* self, f32 dt) {
    fn_71011d8fe4(reinterpret_cast<UnkNewGameScreen*>(self), dt);
    UnkPagedList_ovb0108* list = self->list;
    s32 page = list->page;
    if (!self->changed || page == -1)
        return;
    if (page + 1 == list->count) {
        fn_71011cdc30(self->next, false);
    } else {
        fn_71011cdc30(self->next, true);
        PlayAnimation(self->next, "ContinueArrowLoop");
    }
    if (page >= 1) {
        fn_71011cdc30(self->prev, true);
        PlayAnimation(self->prev, "ContinueArrowLoop");
    } else {
        fn_71011cdc30(self->prev, false);
    }
    if (self->list->count > self->list->visible)
        fn_71011cca3c(self->scroll, self->page_step * (f32)page);
    self->changed = false;
}

// GUI::CPopUpGeneric's unit (tools/tu_map.py): hides the pop-up while it is shown (+0x3d8). At once:
// the content (the object the holder at +0x240 refers to) is switched off (fn_71011cdce8) and taken
// out of the container at +0x250 (fn_71011d932c), then fn_7100b61c74 finishes. Otherwise "OnHide" is
// sent, the object at +0x258 fades (fn_71011cec68, fn_71011ce208), the content is taken out the same
// way, the state key g_unk_71729f0180 is reported (fn_7100fc9d78), the watch at +0x3a8 is dropped and
// the one at +0x3c0 moved to the set at +0x18 of the +0x30 of the object at +0x248. Names and layouts
// are placeholders.
#include "base/UnkDeferredSet.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0180;

void fn_71011cec68(UnkGuiDisplayObject* self, f32 value);
void fn_71011ce208(UnkGuiDisplayObject* self, f32 value);

struct UnkDisplayObjectContainer_ovb0032;
struct UnkDisplayObject_ovb0032;

struct UnkPopUpHolder_ovb0133 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* object;  // 0x10
};

struct UnkPopUpWatched_ovb0133 {
    u8 pad_0x0[0x30];
    char* owner;  // 0x30
};

struct UnkPopUpGeneric_ovb0133 {
    void** vtable;
    u8 pad_0x8[0x238];
    UnkPopUpHolder_ovb0133* content;  // 0x240
    UnkPopUpWatched_ovb0133* watched;  // 0x248
    UnkDisplayObjectContainer_ovb0032* container;  // 0x250
    UnkGuiDisplayObject* fade;  // 0x258
    u8 pad_0x260[0x148];
    base::UnkDeferredSet<void>* watch_0x3a8;
    u8 pad_0x3b0[0x10];
    base::UnkDeferredSet<void>* watch_0x3c0;
    u8 pad_0x3c8[0x10];
    bool shown;  // 0x3d8

    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkPopUpGeneric_ovb0133*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
};
static_assert(__builtin_offsetof(UnkPopUpGeneric_ovb0133, shown) == 0x3d8, "layout");

void fn_7100b61c74(UnkPopUpGeneric_ovb0133* self, bool flag);

static inline void RemoveContent(UnkPopUpGeneric_ovb0133* self) {
    if (self->content && self->content->object) {
        fn_71011cdce8(self->content->object, false);
        fn_71011d932c(self->container, reinterpret_cast<UnkDisplayObject_ovb0032*>(
                                           self->content ? self->content->object : nullptr));
    }
}

void fn_7100b61b0c(UnkPopUpGeneric_ovb0133* self, bool immediate) {
    if (!self->shown)
        return;
    self->shown = false;
    if (immediate) {
        RemoveContent(self);
        fn_7100b61c74(self, false);
        return;
    }
    self->SendEvent("OnHide", false);
    fn_71011cec68(self->fade, 0.0f);
    fn_71011ce208(self->fade, 0.3f);
    RemoveContent(self);
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
    if (self->watch_0x3a8) {
        fn_71004f3ea4(self->watch_0x3a8, &self->watch_0x3a8);
        self->watch_0x3a8 = nullptr;
    }
    if (UnkPopUpWatched_ovb0133* watched = self->watched) {
        if (char* owner = watched->owner) {
            base::UnkDeferredSet<void>** slot = &self->watch_0x3c0;
            if (*slot) {
                fn_71004f3ea4(*slot, slot);
                *slot = nullptr;
            }
            *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(owner + 0x18);
            fn_71004f3cf0(*slot, slot);
        }
    }
}

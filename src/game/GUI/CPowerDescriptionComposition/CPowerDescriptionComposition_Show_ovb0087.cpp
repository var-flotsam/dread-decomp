// GUI::CPowerDescriptionComposition's unit (tools/tu_map.py): more members of the composition (see
// CPowerDescriptionComposition_Hide_b49.cpp, _Init_b49.cpp and _b43.cpp): setting the music mode (+0x328: the old one,
// unless 79, handed back through fn_7100f011c4, a new one in 44..71 started through fn_7100f01838 on the music manager
// created on first use), the input handler (the description label (+0x2d8) scrolls by its vf_0x150 / vf_0x158 on
// event kinds 7/15 and 8/16; a changed line updates the continue arrows, the scroll handle and posts
// g_unk_71729f0298), the show (focus remembered at +0x2b8 as a weak handle, "OnShow", the modal panel's set watched
// at +0x310), the close (event 0x2b to the on-close set at +0x230, focus handed back, the Samus menu's flag cleared
// when the focus was a GUI::CList in menu state 8, else the environment manager told) and the destructor. Names and
// layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CArchivesInspectorComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2578.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using UnkGuiObj = UnkGuiDisplayObject;

// The music manager (as src/base/input/UnkPadVibration.cpp declares it).
struct UnkStruct_0x128;

static inline UnkStruct_0x128* Music() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (!m) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

struct UnkPowerDescCompositionMusic {
    u8 pad_0x0[0x328];
    u32 music;  // 0x328
};

void fn_7100b641cc(UnkPowerDescCompositionMusic* self, u32 mode) {
    if (self->music != 79)
        fn_7100f011c4(Music(), self->music);
    self->music = mode;
    if (mode - 44 <= 27)
        fn_7100f01838(Music(), self->music);
}

struct UnkChangeMenuStateButton_b48a3;
struct UnkGuiEvent_b48a3;

struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0298;

struct UnkPowerDescScrollEvent {
    u8 pad_0x0[0x8];
    s32 kind;  // 0x8
    u8 pad_0xc[0xe];
    bool handled;  // 0x1a
};

struct UnkPowerDescText {
    virtual void vf_0x0();
    u8 pad_0x8[0x184];
    s32 last_line;  // 0x18c
    s32 line;       // 0x190

    void ScrollDown(UnkPowerDescScrollEvent* event) {
        reinterpret_cast<void (*)(UnkPowerDescText*, UnkPowerDescScrollEvent*)>(
            (*reinterpret_cast<void***>(this))[0x150 / 8])(this, event);
    }
    void ScrollUp(UnkPowerDescScrollEvent* event) {
        reinterpret_cast<void (*)(UnkPowerDescText*, UnkPowerDescScrollEvent*)>(
            (*reinterpret_cast<void***>(this))[0x158 / 8])(this, event);
    }
};

struct UnkPowerDescScrollBar {
    u8 pad_0x0[0xb5];
    bool visible;  // 0xb5
};

struct UnkPowerDescCompositionScroll {
    u8 pad_0x0[0x2c0];
    UnkPowerDescScrollBar* scroll_bar;   // 0x2c0
    u8 pad_0x2c8[0x8];
    UnkGuiDisplayObject* handle;         // 0x2d0
    UnkPowerDescText* description;       // 0x2d8
    f32 step;                            // 0x2e0
    UnkGuiDisplayObject* continue_down;  // 0x2e8
    UnkGuiDisplayObject* continue_up;    // 0x2f0
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionScroll, continue_up) == 0x2f0, "layout");

static inline void PlayAnimation(UnkGuiDisplayObject* object, const char* name, s32 flags) {
    typedef void (*Fn)(UnkGuiDisplayObject*, const char*, s32);
    (*reinterpret_cast<Fn*>(*reinterpret_cast<char**>(object) + 0x1e8))(object, name, flags);
}

void fn_7100b63ef4(UnkPowerDescCompositionScroll* self, UnkPowerDescScrollEvent* event) {
    fn_71011cf39c(reinterpret_cast<UnkChangeMenuStateButton_b48a3*>(self),
                  reinterpret_cast<UnkGuiEvent_b48a3*>(event));
    if (self->scroll_bar->visible) {
        UnkPowerDescText* text = self->description;
        s32 old = text->line;
        switch (event->kind) {
        case 7:
        case 15:
            text->ScrollDown(event);
            break;
        case 8:
        case 16:
            text->ScrollUp(event);
            break;
        }
        s32 line = self->description->line;
        if (line != old) {
            if (line == self->description->last_line) {
                fn_71011cdc30(self->continue_down, false);
            } else {
                fn_71011cdc30(self->continue_down, true);
                PlayAnimation(self->continue_down, "ContinueArrow_Loop", 0);
            }
            if (line >= 1) {
                fn_71011cdc30(self->continue_up, true);
                PlayAnimation(self->continue_up, "ContinueArrow_Loop", 0);
            } else {
                fn_71011cdc30(self->continue_up, false);
            }
            fn_71011ccd20(self->handle, self->step * (f32)self->description->line);
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0298), self);
        }
    }
    event->handled = true;
}

UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);

static inline UnkGuiObj* FindByPath(void* self, const CRntString& path) {
    return fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
}

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateKey g_unk_71729f0178;

struct UnkPowerDescCompositionShow {
    u8 pad_0x0[0x2b0];
    bool shown;  // 0x2b0
    u8 pad_0x2b1[0x7];
    UnkWeakHandle* focus;  // 0x2b8
    u8 pad_0x2c0[0x38];
    base::UnkDeferredSet<void>* watch_0x2f8;
    u8 pad_0x300[0x10];
    base::UnkDeferredSet<void>* watch_0x310;

    UnkGuiDisplayObject* AsObject() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkPowerDescCompositionShow*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, flag);
    }
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionShow, watch_0x310) == 0x310, "layout");

static inline UnkGuiDisplayObject* AsContainer(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011d7b84()))
               ? obj
               : nullptr;
}

void fn_7100b63ca4(UnkPowerDescCompositionShow* self) {
    fn_71011cdce8(self->AsObject(), true);
    fn_71011cdc30(self->AsObject(), true);
    if (UnkWeakHandle* focus = g_unk_717271e8f8->field_0x230) {
        if (UnkWeakTarget* target = focus->target)
            AssignRef(&self->focus, GetWeakHandle(target));
    }
    fn_71011cf2a4(self->AsObject(), true);
    fn_71011b6bbc(g_unk_717271e8f8, self->AsObject(), false);
    self->shown = true;
    self->SendEvent("OnShow", false);
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
    UnkGuiDisplayObject* panel = AsContainer(FindByPath(self, CRntString("PopUpComposition.ModalPanel")));
    if (panel) {
        if (self->watch_0x2f8) {
            fn_71004f3ea4(self->watch_0x2f8, &self->watch_0x2f8);
            self->watch_0x2f8 = nullptr;
        }
        base::UnkDeferredSet<void>** slot = &self->watch_0x310;
        char* owner = reinterpret_cast<char* (*)(UnkGuiDisplayObject*)>(
            (*reinterpret_cast<void***>(panel))[0x1f8 / 8])(panel);
        if (*slot) {
            fn_71004f3ea4(*slot, slot);
            *slot = nullptr;
        }
        *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(owner + 0x18);
        fn_71004f3cf0(*slot, slot);
    }
}

struct UnkTutorialRequest;
void fn_71011cd5a0(UnkGuiObj*);

struct UnkPowerDescMenuState {
    u8 pad_0x0[0x2c0];
    UnkGuiDisplayObject* samus_menu;  // 0x2c0
    u8 pad_0x2c8[0x18];
    s32 state;  // 0x2e0
};

struct UnkSamusMenuFlag {
    u8 pad_0x0[0x2f9];
    bool field_0x2f9;
};

struct UnkPowerDescCompositionClose {
    u8 pad_0x0[0x230];
    u8 on_close[0x80];  // 0x230
    bool shown;  // 0x2b0
    u8 pad_0x2b1[0x7];
    UnkWeakHandle* focus;  // 0x2b8

    UnkGuiDisplayObject* AsObject() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    UnkGuiDisplayObject* FocusTarget() {
        return focus ? reinterpret_cast<UnkGuiDisplayObject*>(focus->target) : nullptr;
    }
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionClose, focus) == 0x2b8, "layout");

template <const base::reflection::CClass* (*Class)()>
static inline bool IsA(UnkGuiDisplayObject* obj) {
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                         reinterpret_cast<const base::reflection::CType*>(Class()));
}

static inline void* Environment() {
    void* m = g_unk_7101d47368;
    if (!m) {
        m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return m;
}

bool fn_7100b64290(UnkPowerDescCompositionClose* self) {
    UnkGuiEvent event;
    {
        s32 kind = 0x2b;
        fn_710120235c(&event, self->AsObject(), &kind, nullptr);
    }
    {
        UnkTutorialRequest* request = reinterpret_cast<UnkTutorialRequest*>(&event);
        fn_7100af3364(self->on_close, &request);
    }
    fn_71011cdce8(self->AsObject(), false);
    fn_71011cdc30(self->AsObject(), false);
    fn_71011cf2a4(self->AsObject(), false);
    self->shown = false;
    UnkPowerDescMenuState* menu = reinterpret_cast<UnkPowerDescMenuState*>(fn_7100deac84(g_unk_7101d47310));
    if (menu->state == 8) {
        UnkWeakHandle* focus = self->focus;
        if (focus && focus->target &&
            IsA<fn_71011fe904>(reinterpret_cast<UnkGuiDisplayObject*>(focus->target))) {
            UnkGuiDisplayObject* samus =
                reinterpret_cast<UnkPowerDescMenuState*>(fn_7100deac84(g_unk_7101d47310))->samus_menu;
            bool is_samus_menu = IsA<fn_7100b65dc0>(samus);
            UnkSamusMenuFlag* flag = reinterpret_cast<UnkSamusMenuFlag*>(is_samus_menu ? samus : nullptr);
            reinterpret_cast<u8*>(flag)[0x2f9] = 0;
            fn_71011cf2a4(self->FocusTarget(), true);
            fn_71011b6bbc(g_unk_717271e8f8, self->FocusTarget(), false);
        }
    } else {
        fn_7100d8e0a0(Environment());
    }
    ResetRef(&self->focus);
    fn_71011cd5a0(self->AsObject());
    fn_7101202488(&event);
    return true;
}

extern void* g_unk_7101a9df38[];

struct UnkPowerDescWatch {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;

    __attribute__((always_inline)) void Destroy() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

struct UnkPowerDescPtrVector {
    void*** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkPowerDescCompositionDtor {
    void* vtable;
    u8 pad_0x8[0x228];
    UnkPowerDescPtrVector links;   // 0x230
    UnkPowerDescPtrVector list_1;  // 0x258
    UnkPowerDescPtrVector list_2;  // 0x280
    u8 pad_0x2a8[0x10];
    UnkRefObject* field_0x2b8;
    u8 pad_0x2c0[0x38];
    UnkPowerDescWatch watch_0x2f8;
    UnkPowerDescWatch watch_0x310;
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionDtor, watch_0x310) == 0x310, "layout");

void fn_7100b63694(UnkPowerDescCompositionDtor* self) {
    self->vtable = &g_unk_7101a9df38[2];
    self->watch_0x310.Destroy();
    self->watch_0x2f8.Destroy();
    if (UnkRefObject* p = self->field_0x2b8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    for (void*** it = self->links.data, ***end = it + self->links.count; it != end; ++it)
        **it = nullptr;
    self->links.count = 0;
    self->list_1.count = 0;
    self->list_2.count = 0;
    self->list_2.Free();
    self->list_1.Free();
    self->links.Free();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

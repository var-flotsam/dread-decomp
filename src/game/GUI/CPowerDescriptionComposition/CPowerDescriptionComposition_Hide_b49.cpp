// GUI::CPowerDescriptionComposition's unit (tools/tu_map.py): hides the composition on an event not yet
// handled (+0x1a, set afterwards): once (+0x2b0), the music mode goes back to 79 (the music manager,
// created on first use, told through fn_7100f011c4 when it changes), "OnHide" is sent, the state key
// g_unk_71729f0180 is reported (fn_7100fc9d78), and when the "PopUpComposition.ModalPanel" is a
// GUI::CDisplayObjectContainer the watch at +0x310 is dropped and the one at +0x2f8 moved to the set
// at +0x18 of what the panel's vf_0x1f8 returns. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CRntString;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);

// Looks a child up by path (declared as src/game_s7/UnkGuiS7.h does: the path string by address).
static inline UnkGuiObj* FindByPath(void* self, const CRntString& path) {
    return fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
}

// The music manager (as src/base/input/UnkPadVibration.cpp declares it).
struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f011c4(UnkStruct_0x128* self, u32 mode);

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0180;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);

static inline UnkStruct_0x128* Music() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (!m) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

struct UnkGuiHideEvent {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkPowerDescCompositionHide {
    u8 pad_0x0[0x2b0];
    bool hidden;  // 0x2b0
    u8 pad_0x2b1[0x47];
    base::UnkDeferredSet<void>* watch_0x2f8;
    u8 pad_0x300[0x10];
    base::UnkDeferredSet<void>* watch_0x310;
    u8 pad_0x318[0x10];
    u32 music;  // 0x328

    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkPowerDescCompositionHide*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, flag);
    }
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionHide, music) == 0x328, "layout");

static inline UnkGuiDisplayObject* AsContainer(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011d7b84()))
               ? obj
               : nullptr;
}

void fn_7100b6404c(UnkPowerDescCompositionHide* self, UnkGuiHideEvent* event) {
    if (event->handled)
        return;
    if (!self->hidden) {
        self->hidden = true;
        if (self->music != 79)
            fn_7100f011c4(Music(), self->music);
        self->music = 79;
        self->SendEvent("OnHide", false);
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
        UnkGuiDisplayObject* panel = AsContainer(FindByPath(self, CRntString("PopUpComposition.ModalPanel")));
        if (panel) {
            if (self->watch_0x310) {
                fn_71004f3ea4(self->watch_0x310, &self->watch_0x310);
                self->watch_0x310 = nullptr;
            }
            base::UnkDeferredSet<void>** slot = &self->watch_0x2f8;
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
    event->handled = true;
}

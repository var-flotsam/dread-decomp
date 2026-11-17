// GUI::CPopUpComposition's unit (tools/tu_map.py): starts the pop-up's entry. Plays the entry
// animation the mode picks (the top or bottom one, the game's own when the game manager says so or the
// composition asks, the reduced one otherwise), shows the fade screen, registers the pop-up and its
// panel in their deferred sets, plays the opening sound and clears the closing flags. Names and layouts
// are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::UnkDeferredSet;
using base::global::CStrId;
using base::global::UnkStrIdInline;

// Whether the game manager plays the full entry animations.
bool fn_7100deac24(UnkGameManager* self);

// The sound event the pop-up's opening plays.
extern UnkStateKey g_unk_71729f0178;

// What the panel's owner registers the pop-up in.
struct UnkPopUpPanelOwner_b5f938 {
    u8 pad_0x0[0x18];
    UnkDeferredSet<void> registrations;
};

struct UnkPopUpPanel_b5f938 : UnkWeakTarget {
    u8 pad_0x8[0x28];
    UnkPopUpPanelOwner_b5f938* owner;  // 0x30
};

struct UnkPopUpComposition_b5f938 : UnkGuiDisplayObject {
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0(const CStrId* animation);  // plays the animation

    u8 pad_0x198[0x98];
    UnkDeferredSet<void>* registration;        // 0x230
    u8 pad_0x238[0x10];
    UnkDeferredSet<void>* panel_registration;  // 0x248
    u8 pad_0x250[0x93];
    bool field_0x2e3;
    bool field_0x2e4;
    u8 pad_0x2e5[0x33];
    UnkWeakHandle* panel;        // 0x318
    UnkWeakHandle* fade_screen;  // 0x320
    u8 pad_0x328[0x78];
    bool field_0x3a0;
};
static_assert(__builtin_offsetof(UnkPopUpComposition_b5f938, registration) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkPopUpComposition_b5f938, field_0x2e3) == 0x2e3, "layout");
static_assert(__builtin_offsetof(UnkPopUpComposition_b5f938, panel) == 0x318, "layout");
static_assert(__builtin_offsetof(UnkPopUpComposition_b5f938, field_0x3a0) == 0x3a0, "layout");

struct UnkPopUpComposition_ovb0085;

static inline void Unregister(UnkDeferredSet<void>** slot) {
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
}

void fn_7100b5f938(UnkPopUpComposition_ovb0085* composition, u32 mode) {
    auto* self = reinterpret_cast<UnkPopUpComposition_b5f938*>(composition);
    static CStrId s_reduced("OnEnter_reduced", true);
    static CStrId s_reduced_top("OnEnter_reduced_Top", true);
    static CStrId s_reduced_bottom("OnEnter_reduced_Bottom", true);

    CStrId animation(UnkStrIdInline(), nullptr, true);
    switch (mode) {
    case 2:
        animation = s_reduced_bottom;
        break;
    case 1:
        animation = s_reduced_top;
        break;
    default:
        if (fn_7100deac24(g_unk_7101d47310) || self->field_0x2e3)
            animation = *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1ab8);
        else
            animation = s_reduced;
        break;
    }
    self->vf_0x1f0(&animation);

    if (self->fade_screen) {
        if (UnkWeakTarget* fade = self->fade_screen->target) {
            fn_71011cdc30(reinterpret_cast<UnkGuiDisplayObject*>(fade), !self->field_0x2e4 && mode == 0);
            self->field_0x2e4 = false;
            fn_71011cd5a0(self);
        }
    }
    Unregister(&self->registration);
    if (self->panel) {
        if (UnkPopUpPanel_b5f938* panel = static_cast<UnkPopUpPanel_b5f938*>(self->panel->target)) {
            if (UnkPopUpPanelOwner_b5f938* owner = panel->owner) {
                Unregister(&self->panel_registration);
                self->panel_registration = &owner->registrations;
                fn_71004f3cf0(self->panel_registration, &self->panel_registration);
            }
        }
    }
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
    self->field_0x3a0 = false;
    self->field_0x2e3 = false;
}

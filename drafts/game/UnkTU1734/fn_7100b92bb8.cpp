// Unit 1734 of tools/tu_map.py (no class hint): a screen's timed reveal. Every step shows one more of its
// display objects once its timer has run (1, 0.5 and 1 seconds), with a sound; the last step also takes the
// transition settings from the global ones; after that, the transition runs. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "game/CBossAIComponent.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2567.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;

struct UnkEnumType;
extern UnkEnumType g_unk_71729f0228;
extern UnkEnumType g_unk_71729f0278;

void fn_7100eb7358(void* object, u32 a, u32 b);

struct UnkPanelState_lb92bb8;
void fn_71011ce488(UnkGuiDisplayObject* self, UnkPanelState_lb92bb8* state);

// A display object as the reveal sees it: the state it is reset to at +0xa0.
struct UnkPanel_lb92bb8 {
    u8 pad_0x0[0xa0];
    UnkPanelState_lb92bb8* state() { return reinterpret_cast<UnkPanelState_lb92bb8*>(pad_0x0 + 0xa0); }
};

struct UnkFadeKey_lb92bb8 {
    u32 field_0x0;
    f32 field_0x4;
    u32 field_0x8;
};

struct UnkFadeSettings_lb92bb8 {
    UnkFadeKey_lb92bb8 key;  // 0x0
    u8 pad_0xc[0x4];
    CRntString name;  // 0x10
    u8 pad_0x40[0x38];
    u32 field_0x78;

    bool operator==(const UnkFadeSettings_lb92bb8& other) const {
        return key.field_0x4 == other.key.field_0x4 && fn_710007f220(&name, &other.name);
    }

    // Takes the other settings' key, name and +0x78. The original compares the two first and ignores the
    // result.
    void Set(const UnkFadeSettings_lb92bb8& other) {
        static_cast<void>(other == *this);
        key = other.key;
        fn_710007eaa0(&name, &other.name);
        field_0x78 = other.field_0x78;
    }
};

extern UnkFadeSettings_lb92bb8 g_unk_7172769c08;

struct UnkModeFlags_lb92bb8 {
    u8 pad_0x0[0xc68];
    bool field_0xc68;
};

struct UnkTransition_lb92bb8 {
    u8 pad_0x0[0x50];
    UnkFadeSettings_lb92bb8 settings;  // 0x50
    f32 alpha;                         // 0xd0
    u8 pad_0xd4[0x1];
    bool running;  // 0xd5
};

struct UnkReveal_lb92bb8 {
    u8 pad_0x0[0x30];
    void* sound_state;  // 0x30
    u8 pad_0x38[0x18];
    UnkGuiDisplayObject* first;   // 0x50
    UnkGuiDisplayObject* second;  // 0x58
    UnkGuiDisplayObject* field_0x60;
    UnkGuiDisplayObject* faded;  // 0x68
    u8 pad_0x70[0x8];
    UnkGuiDisplayObject* field_0x78;
    UnkGuiDisplayObject* field_0x80;
    bool field_0x88;
    u8 pad_0x89[0x3];
    s32 step;    // 0x8c
    f32 timer;   // 0x90
    u8 pad_0x94[0x4];
    UnkTransition_lb92bb8 transition;  // 0x98
};

static_assert(__builtin_offsetof(UnkReveal_lb92bb8, transition.running) == 0x16d, "layout");

// Takes the member, not its value: each call may change it, and the original reads it again for each.
inline void Show(UnkGuiDisplayObject*& object) {
    fn_71011ce488(object, reinterpret_cast<UnkPanel_lb92bb8*>(object)->state());
    fn_71011cec68(object, 1.0f);
    fn_71011ce208(object, 0.2f);
}

void fn_7100b92bb8(UnkReveal_lb92bb8* self, f32 dt) {
    switch (self->step) {
    case 0:
        self->timer += dt;
        if (!(self->timer > 1.0f))
            return;
        self->timer = 0.0f;
        Show(self->first);
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0228), self->sound_state);
        ++self->step;
        break;
    case 1:
        self->timer += dt;
        if (!(self->timer > 0.5f))
            return;
        self->timer = 0.0f;
        Show(self->second);
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0228), self->sound_state);
        ++self->step;
        break;
    case 2:
        self->timer += dt;
        if (!(self->timer > 1.0f))
            return;
        Show(self->field_0x78);
        Show(self->field_0x80);
        Show(self->field_0x60);
        if (reinterpret_cast<UnkModeFlags_lb92bb8*>(g_unk_71726bc570)->field_0xc68) {
            UnkTransition_lb92bb8* transition = &self->transition;
            transition->settings.Set(g_unk_7172769c08);
            if (transition->running)
                fn_7100eb7358(transition, 0, 1);
            fn_7100eb7358(transition, 1, 0);
            fn_71011cfba8(self->faded, transition->alpha);
        }
        {
            UnkEnumType* sound = !self->field_0x88 ? &g_unk_71729f0228 : &g_unk_71729f0278;
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(sound), self->sound_state);
        }
        self->step = -1;
        break;
    default:
        if (self->transition.running) {
            fn_7100eb7470(&self->transition, dt);
            fn_71011cfba8(self->faded, self->transition.alpha);
        }
        break;
    }
}

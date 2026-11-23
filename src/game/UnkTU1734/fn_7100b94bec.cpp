// Unit 1734 of tools/tu_map.py (no class hint): a screen's reveal, one step per call: steps 0 to 3 each wait
// out a delay and then bring in one more object (with a sound); step 4 brings in the last two, applies the
// text style and plays the closing sound. The text block's transition runs while it is in use. Names and
// layouts are placeholders.
#include "../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2567.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;

using unk_static_init::UnkEnumType;

// A text style's three numbers, copied as one.
struct UnkTextParams_lb94bec {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkTextStyle_lb94bec {
    UnkTextParams_lb94bec params;
    u8 pad_0xc[0x4];
    CRntString font;  // 0x10
    u8 pad_0x40[0x38];
    u32 field_0x78;
};
static_assert(__builtin_offsetof(UnkTextStyle_lb94bec, field_0x78) == 0x78, "UnkTextStyle_lb94bec layout");
extern UnkTextStyle_lb94bec g_unk_7172769c08;  // the text style

// A display object as far as this file uses it: its colour at +0xa0.
struct UnkGuiObjView_lb94bec {
    u8 pad_0x0[0xa0];
    unk_gui::UnkColor color;  // 0xa0
};

struct UnkTextBlock_lb94bec {
    u8 pad_0x0[0x50];
    UnkTextParams_lb94bec params;  // 0x50
    u8 pad_0x5c[0x4];
    CRntString font;  // 0x60
    u8 pad_0x90[0x38];
    u32 field_0xc8;
    u8 pad_0xcc[0x4];
    f32 alpha;  // 0xd0
    u8 pad_0xd4[0x1];
    bool in_use;  // 0xd5
};
static_assert(__builtin_offsetof(UnkTextBlock_lb94bec, field_0xc8) == 0xc8, "UnkTextBlock_lb94bec layout");
static_assert(__builtin_offsetof(UnkTextBlock_lb94bec, in_use) == 0xd5, "UnkTextBlock_lb94bec layout");

struct UnkReveal_lb94bec {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30: what the sounds play for
    u8 pad_0x38[0x10];
    UnkGuiDisplayObject* logo;  // 0x48
    UnkGuiDisplayObject* items[4];  // 0x50
    u8 pad_0x70[0x8];
    UnkGuiObj* label;  // 0x78
    u8 pad_0x80[0x8];
    UnkGuiDisplayObject* footer;  // 0x88
    bool alternate;  // 0x90
    u8 pad_0x91[0x3];
    s32 step;  // 0x94
    f32 timer;  // 0x98
    u8 pad_0x9c[0x4];
    UnkTextBlock_lb94bec text;  // 0xa0
};
static_assert(__builtin_offsetof(UnkReveal_lb94bec, text) == 0xa0, "UnkReveal_lb94bec layout");

// Copies a style into a text block; returns whether it changed the block's second number or font name.
static inline bool ApplyStyle(UnkTextBlock_lb94bec* block, const UnkTextStyle_lb94bec* style) {
    bool changed = style->params.field_0x4 != block->params.field_0x4 || CRntString_KeysDiffer(&style->font, &block->font);
    block->params = style->params;
    CRntString_Assign(&block->font, &style->font);
    block->field_0xc8 = style->field_0x78;
    return changed;
}

// Brings an object in: back to its own colour, fully shown, fading in.
static inline void BringIn(UnkGuiDisplayObject*& object) {
    fn_71011ce488(object, &reinterpret_cast<UnkGuiObjView_lb94bec*>(object)->color);
    fn_71011cec68(object, 1.0f);
    fn_71011ce208(object, 0.2f);
}

static inline void PlaySound(UnkReveal_lb94bec* self, UnkEnumType* event) {
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(event), self->owner);
}

// Steps 0 to 3: once the delay has passed, the step's object comes in.
static inline void RevealItem(UnkReveal_lb94bec* self, u32 index, f32 delay, f32 dt) {
    self->timer += dt;
    if (self->timer > delay) {
        self->timer = 0.0f;
        BringIn(self->items[index]);
        PlaySound(self, &g_unk_71729f0228);
        self->step++;
    }
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100b94bec(UnkReveal_lb94bec* self, f32 dt) {
    switch (self->step) {
    case 0:
        RevealItem(self, 0, 1.0f, dt);
        break;
    case 1:
        RevealItem(self, 1, 0.5f, dt);
        break;
    case 2:
        RevealItem(self, 2, 1.0f, dt);
        break;
    case 3:
        RevealItem(self, 3, 0.5f, dt);
        break;
    case 4:
        self->timer += dt;
        if (self->timer > 1.0f) {
            BringIn(self->logo);
            BringIn(self->footer);
            // The block is refreshed whether or not the style changed it.
            ApplyStyle(&self->text, &g_unk_7172769c08);
            if (self->text.in_use)
                unk_gui::fn_7100eb7358(&self->text, 0, 1);
            unk_gui::fn_7100eb7358(&self->text, 1, 0);
            fn_71011cfba8(self->label, self->text.alpha);
            PlaySound(self, !self->alternate ? &g_unk_71729f0228 : &g_unk_71729f0278);
            self->step = -1;
        }
        break;
    }
    if (self->text.in_use) {
        fn_7100eb7470(&self->text, dt);
        fn_71011cfba8(self->label, self->text.alpha);
    }
}

// Unit 1734 of tools/tu_map.py (no class hint): a menu's reveal, one step per call: step 0 brings in the title,
// steps 1 to 12 each bring in one button of the two button lists (six each) and show its highlight, step 13
// brings in one more object and step 14 the last two, applying the text style and playing the closing sound.
// Afterwards the text block's transition runs while it is in use. Names and layouts are placeholders.
#include "../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
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

// A text style's three numbers, copied as one.
struct UnkTextParams_lb93f54 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkTextStyle_lb93f54 {
    UnkTextParams_lb93f54 params;
    u8 pad_0xc[0x4];
    CRntString font;  // 0x10
    u8 pad_0x40[0x38];
    u32 field_0x78;
};
static_assert(__builtin_offsetof(UnkTextStyle_lb93f54, field_0x78) == 0x78, "UnkTextStyle_lb93f54 layout");
extern UnkTextStyle_lb93f54 g_unk_7172769c08;  // the text style

// A display object as far as this file uses it: its rest state at +0xa0.
struct UnkGuiObjView_lb93f54 {
    u8 pad_0x0[0xa0];
    u8 rest[0x14];  // 0xa0
};
void fn_71011ce488(UnkGuiDisplayObject* self, const void* state);

struct UnkTextBlock_lb93f54 {
    u8 pad_0x0[0x50];
    UnkTextParams_lb93f54 params;  // 0x50
    u8 pad_0x5c[0x4];
    CRntString font;  // 0x60
    u8 pad_0x90[0x38];
    u32 field_0xc8;
    u8 pad_0xcc[0x4];
    f32 alpha;  // 0xd0
    u8 pad_0xd4[0x1];
    bool in_use;  // 0xd5
};
static_assert(__builtin_offsetof(UnkTextBlock_lb93f54, field_0xc8) == 0xc8, "UnkTextBlock_lb93f54 layout");
static_assert(__builtin_offsetof(UnkTextBlock_lb93f54, in_use) == 0xd5, "UnkTextBlock_lb93f54 layout");

UnkGuiDisplayObject* fn_71011d99ac(UnkGuiDisplayObject* container, u64 id);

// A list of display objects, linked through `next`.
struct UnkGuiListNode_lb93f54 {
    UnkGuiDisplayObject* object;
    UnkGuiListNode_lb93f54* next;
};

struct UnkButtonList_lb93f54 {
    u8 pad_0x0[0x1d0];
    UnkGuiListNode_lb93f54* first;  // 0x1d0

    UnkGuiDisplayObject* At(u32 index) {
        UnkGuiListNode_lb93f54* node = first;
        for (u32 i = 0; i < index; ++i)
            node = node->next;
        return node->object;
    }
};

struct UnkMenuReveal_lb93f54 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30: what the sounds play for
    u8 pad_0x38[0x8];
    UnkGuiDisplayObject* frame;  // 0x40
    UnkGuiDisplayObject* title;  // 0x48
    UnkGuiDisplayObject* footer;  // 0x50
    UnkGuiObj* label;  // 0x58
    UnkGuiDisplayObject* panel;  // 0x60
    UnkButtonList_lb93f54* second_buttons;  // 0x68
    UnkButtonList_lb93f54* first_buttons;  // 0x70
    u8 pad_0x78[0x8];
    bool alternate;  // 0x80
    u8 pad_0x81[0x3];
    s32 step;  // 0x84
    f32 timer;  // 0x88
    u8 pad_0x8c[0x4];
    UnkTextBlock_lb93f54 text;  // 0x90
};
static_assert(__builtin_offsetof(UnkMenuReveal_lb93f54, text) == 0x90, "UnkMenuReveal_lb93f54 layout");

// Copies a style into a text block; returns whether it changed the block's second number or font name.
static inline bool ApplyStyle(UnkTextBlock_lb93f54* block, const UnkTextStyle_lb93f54* style) {
    bool changed = style->params.field_0x4 != block->params.field_0x4 || fn_710007f220(&style->font, &block->font);
    block->params = style->params;
    fn_710007eaa0(&block->font, &style->font);
    block->field_0xc8 = style->field_0x78;
    return changed;
}

// Brings an object in: back to its rest state, fully shown, fading in.
static inline void BringIn(UnkGuiDisplayObject*& object) {
    fn_71011ce488(object, reinterpret_cast<UnkGuiObjView_lb93f54*>(object)->rest);
    fn_71011cec68(object, 1.0f);
    fn_71011ce208(object, 0.2f);
}

static inline void PlaySound(UnkMenuReveal_lb93f54* self, UnkEnumType* event) {
    fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(event), self->owner);
}

static inline bool IsButton(UnkGuiDisplayObject* object) {
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->GetClass()),
                         reinterpret_cast<const base::reflection::CType*>(fn_71011be400()));
}

// The id of a button's highlight child.
static const u64 kHighlightId = 0xd58e155e79ea2a89;

void fn_7100b93f54(UnkMenuReveal_lb93f54* self, f32 dt) {
    if (self->step == 0) {
        self->timer += dt;
        if (self->timer > 0.5f) {
            self->timer = -0.2f;
            BringIn(self->title);
            PlaySound(self, &g_unk_71729f0228);
            self->step++;
        }
    } else if (self->step >= 1 && self->step <= 12) {
        self->timer += dt;
        if (self->timer > 0.2f) {
            self->timer -= 0.2f;
            UnkButtonList_lb93f54* list;
            u32 index;
            if (self->step < 7) {
                list = self->first_buttons;
                index = self->step - 1;
            } else {
                list = self->second_buttons;
                index = self->step - 7;
            }
            UnkGuiDisplayObject* object = list->At(index);
            UnkGuiDisplayObject* button = object && IsButton(object) ? object : nullptr;
            fn_71011cdce8(button, true);
            UnkGuiDisplayObject* highlight = fn_71011d99ac(button, kHighlightId);
            fn_71011cdc30(highlight, true);
            fn_71011ce208(highlight, 0.3f);
            PlaySound(self, &g_unk_71729f0228);
            self->step++;
        }
    } else if (self->step == 13) {
        self->timer += dt;
        if (self->timer > 0.5f) {
            self->timer = 0.0f;
            BringIn(self->panel);
            PlaySound(self, &g_unk_71729f0228);
            self->step++;
        }
    } else if (self->step == 14) {
        self->timer += dt;
        if (self->timer > 0.5f) {
            BringIn(self->frame);
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
    } else if (self->text.in_use) {
        fn_7100eb7470(&self->text, dt);
        fn_71011cfba8(self->label, self->text.alpha);
    }
}

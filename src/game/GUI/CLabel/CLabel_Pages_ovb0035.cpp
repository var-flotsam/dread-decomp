// GUI::CLabel unit (tools/tu_map.py unit 2752, the name from reflection): the label's set-up and
// destructor, its update (language, layout, pages and scrolling), its page buttons, its line and
// run bookkeeping (the lines at +0x2c0, a list of lines each holding a list of runs, each run a text
// object with its glyphs) and a number shown as its text. The layouts follow
// src/game/GUI/CLabel.cpp's UnkLabelLayout, UnkLabelLine, UnkLabelRun and UnkLabelGlyph. Names and
// layouts are placeholders.
#include <math.h>
#include <new>
#include <wchar.h>

#include "../../../game_s7/UnkGuiS7.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/math/CVector.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU135.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CRntString;
using base::global::CStrId;

// As src/game/GUI/CLabel.cpp declares them.
struct UnkLayoutText;
struct UnkLabelLayout;
void* fn_71011b882c(UnkGuiRoot*, const CStrId*);
void fn_71011bab80(UnkGuiRoot* root, void* object, s32 frame);

const CStrId& fn_71011b8920(UnkGuiRoot* root);
void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);
struct UnkSprite_spr;
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);
struct UnkLabelFontView_b52;
s32 fn_71011eedd0(UnkLabelFontView_b52* self);

// The language singleton (src/game/UnkLanguage.cpp) and its current language.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;

// ---------------------------------------------------------------------------------------------
// A number shown as the label's text.

void unk_gui::fn_71011ed0f0(UnkGuiObj* label, s32 value) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%d", value);
    UnkGuiText text;
    fn_7100066a50(&text, reinterpret_cast<const char32_t*>(buf));
    fn_71011ec468(label, text, false);
    fn_7100066e44(&text);
}

// ---------------------------------------------------------------------------------------------
// The label's set-up on the display object's: empty texts, an empty font name and the GUI root's
// default font, one line of 1000 runs, the alignments and the scroll state.

struct UnkLabelInit_b0035 {
    u8 base[0x185];
    bool field_0x185;
    u8 pad_0x186[0x92];
    u8 text[0x20];         // 0x218
    u8 source_text[0x20];  // 0x238
    bool dirty;            // 0x258
    u8 pad_0x259[7];
    CStrId font_id;        // 0x260
    CRntString font_name;  // 0x268
    u8 pad_0x268[0x30 - sizeof(CRntString)];
    void* font;            // 0x298
    bool field_0x2a0;
    u8 pad_0x2a1[3];
    s32 step;              // 0x2a4
    f32 max_width;         // 0x2a8
    u32 max_runs;          // 0x2ac
    s32 space;             // 0x2b0
    u32 h_align;           // 0x2b4
    u8 pad_0x2b8[0x38];
    s32 line;              // 0x2f0
    bool field_0x2f4;
    u8 pad_0x2f5[7];
    bool tags;             // 0x2fc
    u8 pad_0x2fd[0xb];
    s32 fixed_advance;     // 0x308
    u32 field_0x30c;
    s32 field_0x310;
    u32 field_0x314;
    f32 field_0x318;
    f32 field_0x31c;
    u8 pad_0x320[8];
    u32 v_align;           // 0x328
};
static_assert(__builtin_offsetof(UnkLabelInit_b0035, v_align) == 0x328, "UnkLabelInit_b0035 layout");

void fn_71011ec1f4(UnkLabelInit_b0035* self) {
    fn_71011cd4b4(self);
    self->field_0x185 = true;
    fn_7100066af0(reinterpret_cast<UnkLayoutText*>(self->text), U"");
    fn_7100066af0(reinterpret_cast<UnkLayoutText*>(self->source_text), U"");
    self->max_width = 1.0f;
    self->max_runs = 1000;
    self->dirty = true;
    self->h_align = 1;
    self->v_align = 1;
    self->field_0x2a0 = false;
    self->line = -1;
    self->field_0x2f4 = false;
    self->tags = true;
    fn_710007e77c(&self->font_name, "");
    self->font_id = fn_71011b8920(g_unk_717271e8f8);
    self->fixed_advance = -1;
    self->field_0x30c = 0;
    self->field_0x310 = -1;
    self->field_0x314 = 0;
    self->field_0x318 = -1.0f;
    self->field_0x31c = 1.0f;
    self->step = 2;
}

// ---------------------------------------------------------------------------------------------
// The label's destructor: the line list's nodes, the font name and id, the two texts and the
// callback set at +0x198 (its callbacks are detached first), then the display object's.

// A callback of the set: its first word points back at the set.
struct UnkLabelCallback_b0035 {
    void* set;
};

struct UnkLabelDtorLineNode_b0035 {
    void* line;
    UnkLabelDtorLineNode_b0035* next;
};

struct UnkLabelDtor_b0035 {
    char* vtable;
    u8 pad_0x8[0x190];
    base::UnkVector<UnkLabelCallback_b0035*> items;           // 0x198
    base::UnkVector<UnkLabelCallback_b0035*> pending_add;     // 0x1c0
    base::UnkVector<UnkLabelCallback_b0035*> pending_remove;  // 0x1e8
    bool locked;                                              // 0x210
    u8 pad_0x211[7];
    u8 text[0x20];         // 0x218
    u8 source_text[0x20];  // 0x238
    u8 pad_0x258[8];
    base::UnkStrIdMember font_id;  // 0x260
    CRntString font_name;          // 0x268
    u8 pad_0x268[0x58 - sizeof(CRntString)];
    char* lines_vtable;  // 0x2c0
    u32 line_count;      // 0x2c8
    u8 pad_0x2cc[4];
    UnkLabelDtorLineNode_b0035* lines;  // 0x2d0
    UnkLabelDtorLineNode_b0035* lines_tail;
    base::UnkAllocator* allocator;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkLabelDtor_b0035, allocator) == 0x2e0, "UnkLabelDtor_b0035 layout");

void fn_71011ebfa0(UnkLabelDtor_b0035* self) {
    self->vtable = g_unk_7101cb6380 + 0x10;
    self->lines_vtable = g_unk_7101cb65e8 + 0x10;
    for (UnkLabelDtorLineNode_b0035* n = self->lines; n;) {
        UnkLabelDtorLineNode_b0035* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->line_count = 0;
    self->lines = nullptr;
    self->lines_tail = nullptr;
    fn_710007ecd4(&self->font_name);
    self->font_id.~UnkStrIdMemberOf();
    fn_7100066e44(self->source_text);
    fn_7100066e44(self->text);
    for (UnkLabelCallback_b0035 **it = self->items.data, **end = it + self->items.count; it != end; ++it)
        (*it)->set = nullptr;
    self->items.count = 0;
    self->pending_add.count = 0;
    self->pending_remove.count = 0;
    self->pending_remove.Free();
    self->pending_add.Free();
    self->items.Free();
    fn_71011ccfb4(reinterpret_cast<char*>(self));
}

// ---------------------------------------------------------------------------------------------
// The runs and their glyphs, and dropping all lines.

void fn_71011baba0(UnkGuiRoot* root, void* object, s32 frame);

// A glyph of a run (0x30 bytes): its id and the object made for it in a frame.
struct UnkLabelGlyph_b0035 {
    u32 kind;
    base::UnkStrIdMember id;  // 0x8
    u8 pad_0x10[0x14];
    s32 frame;     // 0x24
    void* object;  // 0x28

    ~UnkLabelGlyph_b0035() {
        if (object && frame != 0x7fffffff) {
            fn_71011bab80(g_unk_717271e8f8, object, frame);
            object = nullptr;
        }
    }
};
static_assert(sizeof(UnkLabelGlyph_b0035) == 0x30, "UnkLabelGlyph_b0035 layout");

// A run (0x70 bytes): its frame, its text, its glyphs and its text object.
struct UnkLabelRun_b0035 {
    s32 frame;
    u8 pad_0x4[4];
    u8 text[0x20];  // 0x8
    u8 pad_0x28[0x18];
    base::UnkVector<UnkLabelGlyph_b0035> glyphs;  // 0x40
    void* object;                                 // 0x68
};
static_assert(__builtin_offsetof(UnkLabelRun_b0035, object) == 0x68, "UnkLabelRun_b0035 layout");

// The run's destructor: gives its text object and its glyphs' objects back to the GUI root.
void fn_71011ef420(UnkLabelRun_b0035* self) {
    if (self->object)
        fn_71011baba0(g_unk_717271e8f8, self->object, self->frame);
    self->glyphs.Free();
    fn_7100066e44(self->text);
}

template <class T>
struct UnkLabelNode_b0035 {
    T* data;
    UnkLabelNode_b0035* next;
};

// A list of owned pointers in nodes from an allocator.
template <class T>
struct UnkLabelList_b0035 {
    UnkLabelNode_b0035<T>* head;
    UnkLabelNode_b0035<T>* tail;
    base::UnkAllocator* allocator;

    void FreeNodes() {
        for (UnkLabelNode_b0035<T>* n = head; n;) {
            UnkLabelNode_b0035<T>* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

// A line: its runs, deleted with it.
struct UnkLabelGroup_b0035 {
    u8 pad_0x0[0x18];
    UnkLabelList_b0035<UnkLabelRun_b0035> runs;  // 0x18

    ~UnkLabelGroup_b0035() {
        for (UnkLabelNode_b0035<UnkLabelRun_b0035>* n = runs.head; n; n = n->next) {
            if (UnkLabelRun_b0035* run = n->data) {
                fn_71011ef420(run);
                fn_7100083c00(run);
                n->data = nullptr;
            }
        }
        runs.FreeNodes();
    }
};

struct UnkLabelGroups_b0035 {
    u8 pad_0x0[0x2c8];
    u32 line_count;  // 0x2c8
    u8 pad_0x2cc[4];
    UnkLabelList_b0035<UnkLabelGroup_b0035> lines;  // 0x2d0
    u8 pad_0x2e8[8];
    s32 line;  // 0x2f0
};
static_assert(__builtin_offsetof(UnkLabelGroups_b0035, line) == 0x2f0, "UnkLabelGroups_b0035 layout");

// Drops the label's lines and their runs.
void fn_71011ec314(UnkLabelLayout* layout) {
    auto* self = reinterpret_cast<UnkLabelGroups_b0035*>(layout);
    for (UnkLabelNode_b0035<UnkLabelGroup_b0035>* n = self->lines.head; n; n = n->next) {
        if (UnkLabelGroup_b0035* group = n->data) {
            group->~UnkLabelGroup_b0035();
            fn_7100083c00(group);
            n->data = nullptr;
        }
    }
    self->lines.FreeNodes();
    self->line_count = 0;
    self->lines.head = nullptr;
    self->lines.tail = nullptr;
    self->line = -1;
}

// ---------------------------------------------------------------------------------------------
// The current line (+0x2f0): the lines left below it, and the label sized to it.

struct UnkLabelLineInfo_b0035 {
    f32 width;
    f32 height;
    u8 pad_0x8[8];
    s32 field_0x10;
};

struct UnkLabelLines_b0035 {
    u8 pad_0x0[0x8c];
    base::math::CVector2D size;  // 0x8c
    u8 pad_0x94[0x20c];
    bool keep_width;  // 0x2a0
    u8 pad_0x2a1[0x27];
    u32 line_count;  // 0x2c8
    u8 pad_0x2cc[4];
    UnkLabelNode_b0035<UnkLabelLineInfo_b0035>* lines;  // 0x2d0
    u8 pad_0x2d8[0x18];
    s32 line;  // 0x2f0
};
static_assert(__builtin_offsetof(UnkLabelLines_b0035, line) == 0x2f0, "UnkLabelLines_b0035 layout");

static inline UnkLabelLineInfo_b0035* LineAt(UnkLabelLines_b0035* self, s32 index) {
    UnkLabelNode_b0035<UnkLabelLineInfo_b0035>* node = self->lines;
    for (s32 i = 0; i < index; ++i)
        node = node->next;
    return node->data;
}

// The current line's +0x10 less the lines that fit the label (fn_71011eedd0), at least 0; 0
// without a current line.
s32 fn_71011ef398(UnkLabelLines_b0035* self) {
    s32 index = self->line;
    if (index < 0)
        return 0;
    s32 value = LineAt(self, index)->field_0x10;
    s32 rest = value - fn_71011eedd0(reinterpret_cast<UnkLabelFontView_b52*>(self));
    return rest > 0 ? rest : 0;
}

// Sizes the label to its current line: its height, and its width unless +0x2a0 keeps it.
void fn_71011ef2e8(UnkLabelLines_b0035* self) {
    if (!self->line_count)
        return;
    s32 index = self->line;
    if (index < 0)
        return;
    base::math::CVector2D size;
    size.x = self->size.x;
    size.y = self->size.y;
    UnkLabelLineInfo_b0035 line = *LineAt(self, index);
    size.y = line.height;
    if (!self->keep_width)
        size.x = line.width;
    fn_71011cf8d0(reinterpret_cast<UnkSprite_spr*>(self), &size);
}

// ---------------------------------------------------------------------------------------------
// The page buttons: the next and the previous page (+0x190 of +0x18c, by +0x2a4; +0x321 wraps
// around), each after the display object's handler, marking the input handled.

struct UnkGuiInput_b0035 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

struct UnkLabelPager_b0035 {
    u8 pad_0x0[0x184];
    bool changed;  // 0x184: as UnkGuiDisplayObject's field_0x184
    u8 pad_0x185[7];
    s32 count;  // 0x18c
    s32 shown;  // 0x190
    u8 pad_0x194[0x110];
    s32 step;  // 0x2a4
    u8 pad_0x2a8[0x58];
    s32 field_0x300;
    u8 pad_0x304[0xc];
    s32 field_0x310;
    u8 pad_0x314[0xd];
    bool wrap;  // 0x321
};
static_assert(__builtin_offsetof(UnkLabelPager_b0035, wrap) == 0x321, "UnkLabelPager_b0035 layout");

void fn_71011cf740(UnkGuiDisplayObject* self, UnkGuiInput_b0035* input);
void fn_71011cf73c(UnkGuiDisplayObject* self, UnkGuiInput_b0035* input);

// Runs vf_0xa0 the first time the label changes.
static inline void Touch_b0035(UnkLabelPager_b0035* pager) {
    auto* self = reinterpret_cast<UnkGuiDisplayObject*>(pager);
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
}

void fn_71011ee9e0(UnkLabelPager_b0035* self, UnkGuiInput_b0035* input) {
    fn_71011cf740(reinterpret_cast<UnkGuiDisplayObject*>(self), input);
    s32 count = self->count;
    if (count >= 1) {
        if (self->shown != count) {
            s32 next = self->shown + self->step;
            self->shown = next < count ? next : count;
        } else if (self->wrap) {
            self->shown = 0;
        }
        if (self->field_0x310 != -1) {
            self->field_0x310 = -1;
            Touch_b0035(self);
        }
        self->field_0x300 = self->shown;
        Touch_b0035(self);
    }
    input->handled = true;
}

void fn_71011ee8f8(UnkLabelPager_b0035* self, UnkGuiInput_b0035* input) {
    fn_71011cf73c(reinterpret_cast<UnkGuiDisplayObject*>(self), input);
    s32 count = self->count;
    if (count >= 1) {
        if (self->shown != 0) {
            s32 prev = self->shown - self->step;
            self->shown = prev > 0 ? prev : 0;
        } else if (self->wrap) {
            self->shown = count;
        }
        if (self->field_0x310 != -1) {
            self->field_0x310 = -1;
            Touch_b0035(self);
        }
        self->field_0x300 = self->shown;
        Touch_b0035(self);
    }
    input->handled = true;
}

// ---------------------------------------------------------------------------------------------
// The font: looked up once in the GUI root's cache by the label's font path (+0x260), while the
// label has a frame and a path.

struct UnkLabelFontView_b0035 {
    u8 pad_0x0[0x260];
    base::UnkStrIdMember font_path;  // 0x260
    u8 pad_0x268[0x30];
    void* font;                      // 0x298
    u8 pad_0x2a0[0x10];
    s32 line_height;                 // 0x2b0: negative to take the font's (its height of ' ')
};

template <class T>
static inline UnkGuiObjectState* State(T* self) {
    return fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

template <class T>
static inline void* Font(T* self) {
    if (!self->font && State(self)->frame != 0x7fffffff && self->font_path.data != &g_unk_7101d47108->empty) {
        auto lookup = reinterpret_cast<void* (*)(UnkGuiRoot*, const base::UnkStrIdMember*)>(fn_71011b882c);
        self->font = lookup(g_unk_717271e8f8, &self->font_path);
    }
    return self->font;
}

// Lays the text out again once the font is known, and refreshes it.
template <class T>
static inline void Relayout(T* self) {
    if (!Font(self))
        return;
    fn_71011ed1e8(reinterpret_cast<UnkLabelLayout*>(self), true);
    fn_71011ec768(reinterpret_cast<UnkLabelLayout*>(self));
}

// The label's line height in its frame's units: 0 without a font.
f32 fn_71011ef738(UnkLabelFontView_b0035* self) {
    void* font = Font(self);
    if (!font)
        return 0.0f;
    f32 height;
    if (self->line_height >= 0)
        height = (f32)self->line_height;
    else
        height = (f32)fn_71001479dc(font, 0x20);
    UnkGuiObjectState* state = State(self);
    return fn_71011b7e7c(g_unk_717271e8f8, state->frame,
                         &static_cast<const base::math::CVector2D&>(base::math::CVector2D{0.0f, height}))
        .y;
}

// ---------------------------------------------------------------------------------------------
// The label's update: after a language change it takes its font again and re-sets its text, lays
// the text out while it is dirty, and takes its page count from vf_0x230 (the first page unless
// +0x320 keeps the shown one).

void fn_71011cee10(UnkGuiDisplayObject* self);

static inline UnkLanguage* Language_b0035() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

struct UnkLabelUpdate_b0035 {
    void** vtable;
    u8 pad_0x8[0x17c];
    bool changed;  // 0x184: as UnkGuiDisplayObject's field_0x184
    u8 pad_0x185[7];
    s32 count;  // 0x18c
    s32 shown;  // 0x190
    u8 pad_0x194[0xa4];
    UnkGuiText source_text;  // 0x238
    bool dirty;              // 0x258
    u8 pad_0x259[7];
    base::UnkStrIdMember font_path;  // 0x260
    u8 pad_0x268[0x30];
    void* font;  // 0x298
    u8 pad_0x2a0[0x60];
    s32 field_0x300;
    u8 pad_0x304[0x1c];
    bool keep_shown;  // 0x320
    u8 pad_0x321[0xb];
    s32 language;  // 0x32c
};
static_assert(__builtin_offsetof(UnkLabelUpdate_b0035, language) == 0x32c, "UnkLabelUpdate_b0035 layout");

void fn_71011ef164(UnkLabelUpdate_b0035* self) {
    auto* object = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011cee10(object);
    if (!self->dirty) {
        fn_71011ec768(reinterpret_cast<UnkLabelLayout*>(self));
        return;
    }
    s32 language = fn_710119c9f4(Language_b0035());
    if (self->language != language) {
        self->language = language;
        auto lookup = reinterpret_cast<void* (*)(UnkGuiRoot*, const base::UnkStrIdMember*)>(fn_71011b882c);
        self->font = lookup(g_unk_717271e8f8, &self->font_path);
        fn_71011ec468(object, self->source_text, true);
    }
    Relayout(self);
    self->count = reinterpret_cast<s32 (*)(UnkLabelUpdate_b0035*)>(self->vtable[0x230 / 8])(self);
    if (!self->keep_shown)
        self->shown = 0;
    self->field_0x300 = self->shown;
    if (!self->changed) {
        self->changed = true;
        object->vf_0xa0();
    }
}

// ---------------------------------------------------------------------------------------------
// Pages and scrolling: the page count is the lines the runs' text objects show (+0x314), the page
// shown +0x310 (up to +0x30c), the scroll position +0x318 (negative when not scrolling) and the
// part of the pages to show +0x31c; the set at +0x198 is told when the last page is reached.

struct UnkLabelCountTextNode_b0035 {
    u8 pad_0x0[0xdc];
    s32 lines;  // 0xdc
};
struct UnkLabelCountObject_b0035 {
    u8 pad_0x0[0x130];
    UnkLabelCountTextNode_b0035* node;  // 0x130
};
struct UnkLabelCountRun_b0035 {
    u8 pad_0x0[0x68];
    UnkLabelCountObject_b0035* object;  // 0x68
};
struct UnkLabelCountLine_b0035 {
    u8 pad_0x0[0x18];
    UnkLabelNode_b0035<UnkLabelCountRun_b0035>* runs;  // 0x18
};

struct UnkLabelScroller_b0035 {
    u8 pad_0x0[0x184];
    bool changed;  // 0x184: as UnkGuiDisplayObject's field_0x184
    u8 pad_0x185[0x13];
    UnkDeferredSet<UnkDeferredCallback<bool>> on_end;  // 0x198
    u8 pad_0x218[0x48];
    base::UnkStrIdMember font_path;  // 0x260
    u8 pad_0x268[0x30];
    void* font;  // 0x298
    u8 pad_0x2a0[0x30];
    UnkLabelNode_b0035<UnkLabelCountLine_b0035>* lines;  // 0x2d0
    u8 pad_0x2d8[0x34];
    s32 last_page;  // 0x30c
    s32 page;       // 0x310
    s32 pages;      // 0x314
    f32 scroll;     // 0x318
    f32 progress;   // 0x31c: 0 to 1, else none
};
static_assert(__builtin_offsetof(UnkLabelScroller_b0035, progress) == 0x31c, "UnkLabelScroller_b0035 layout");

static inline void Touch_b0035(UnkLabelScroller_b0035* self) {
    if (!self->changed) {
        self->changed = true;
        reinterpret_cast<UnkGuiDisplayObject*>(self)->vf_0xa0();
    }
}

// Lays the text out, counts the pages, lays it out again and goes back to the first page.
void fn_71011ef574(UnkLabelScroller_b0035* self) {
    Relayout(self);
    self->pages = 0;
    for (auto* line = self->lines; line; line = line->next) {
        for (auto* run = line->data->runs; run; run = run->next) {
            s32 pages = self->pages + run->data->object->node->lines;
            __builtin_memcpy(&self->pages, &pages, sizeof(pages));  // the original reloads the list after it
        }
    }
    Relayout(self);
    if (self->page != 0 && self->last_page >= 0) {
        self->page = 0;
        Touch_b0035(self);
    }
    self->scroll = 0.0f;
}

void fn_71011cdfa8(UnkGuiDisplayObject* self, f32 dt);

// Shows `page` when it is a page other than `current` that can be shown.
static inline void SetPage(UnkLabelScroller_b0035* self, s32 current, s32 page) {
    if (page >= 0 && current != page && self->last_page >= page) {
        self->page = page;
        Touch_b0035(self);
    }
}

// The label's update: scrolls 33 pages a second while it scrolls, or shows the part of its pages
// +0x31c asks for; then the display object's update.
void fn_71011eeab4(UnkLabelScroller_b0035* self, f32 dt) {
    if (self->scroll >= 0.0f) {
        f32 scroll = dt * 33.0f + self->scroll;
        s32 page = (s32)floorf(scroll);
        s32 pages = self->pages;
        s32 current = self->page;
        self->scroll = scroll;
        if (page < pages) {
            SetPage(self, current, page);
        } else {
            SetPage(self, current, pages);
            self->scroll = -1.0f;
            bool end = true;
            fn_71000303c0(&self->on_end, &end);
        }
    } else {
        f32 progress = self->progress;
        if (progress >= 0.0f && progress <= 1.0f && self->pages >= 1) {
            s32 pages = self->pages;
            s32 page = (s32)ceilf(progress * (f32)pages);
            page = page > 0 ? (page < pages ? page : pages) : 0;
            SetPage(self, self->page, page);
            if (page == self->pages) {
                bool end = true;
                fn_71000303c0(&self->on_end, &end);
            }
        }
    }
    fn_71011cdfa8(reinterpret_cast<UnkGuiDisplayObject*>(self), dt);
}

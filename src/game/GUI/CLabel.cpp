// From unit 2752 of tools/tu_map.py (0x71011e8df8-0x71011f0810, GUI::CLabel by reflection): the
// label's per-frame refresh of the viewports its groups' items show. Names and layouts are
// placeholders; the viewport is src/game/CMinimapManager.cpp's (a scene item at +0x18 whose flag
// at 0xfb, the viewport's +0x113, marks a pending update, and the view at +0x178 whose rectangle
// fn_71001c36c0 applies).
// Holds fn_71011ee5a8.
#include "base/math/CVector.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGuiFrames.h"
#include "types.h"

using base::math::CVector2D;

// The view (the same definition as src/game/CMinimapManager.cpp's).
struct UnkMinimapView {
    u8 unk_0x0[0x90];
    base::math::CVector2D pos;
    base::math::CVector2D size;
};
void fn_71001c36c0(UnkMinimapView* view, const base::math::CVector2D* pos, const base::math::CVector2D* size);
void fn_71001c3770(UnkMinimapView* view, bool value);

struct UnkStruct_710020a5a0;
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020ad90(void* self, s32 value);

// A viewport an entry shows (laid out as src/game/CMinimapManager.cpp's UnkMinimapViewport).
struct UnkLabelViewport {
    u8 unk_0x0[0x18];
    UnkToggle node;  // its flag_0xfb is set once the view is moved or resized
    u8 unk_0x128[0x50];
    UnkMinimapView* view;
};
static_assert(__builtin_offsetof(UnkLabelViewport, view) == 0x178, "UnkLabelViewport placeholder layout");


// What the GUI root finds for an entry's key: its rectangle and the object at +0x18 whose +0x8 the
// viewport is built from.
struct UnkLabelSource {
    u8 unk_0x0[0x8];
    CVector2D pos;
    CVector2D size;
    char* field_0x18;
};

// An item's entry (0x30 bytes): a value that is zero until the entry is resolved, the key it is
// looked up by and the viewport built for it, with the frame it was last refreshed in.
struct UnkLabelEntry {
    s32 field_0x0;
    u8 unk_0x4[0x4];
    u8 key[0x1c];
    s32 frame;
    UnkLabelViewport* viewport;
};
static_assert(sizeof(UnkLabelEntry) == 0x30, "UnkLabelEntry placeholder layout");

struct UnkLabelItemSub {
    u8 unk_0x0[0x11b];
    bool field_0x11b;
    u8 unk_0x11c[0x14];
    void* field_0x130;
};

struct UnkLabelItem {
    u8 unk_0x0[0x10];
    s32* field_0x10;
    u8 unk_0x18[0x28];
    UnkLabelEntry* entries;  // +0x40
    u32 count;               // +0x48
    u8 unk_0x4c[0x1c];
    UnkLabelItemSub* field_0x68;
};

struct UnkLabelItemNode {
    UnkLabelItem* item;
    UnkLabelItemNode* next;
};
struct UnkLabelGroup {
    u8 unk_0x0[0x18];
    UnkLabelItemNode* items;
};
struct UnkLabelGroupNode {
    UnkLabelGroup* group;
    UnkLabelGroupNode* next;
};

struct UnkLabel {
    u8 unk_0x0[0x13c];
    s32 field_0x13c;
    s32 field_0x140;  // 0x7fffffff when unset: field_0x13c is used instead
    u8 unk_0x144[0x18c];
    UnkLabelGroupNode* groups;  // +0x2d0
};
static_assert(__builtin_offsetof(UnkLabel, groups) == 0x2d0, "UnkLabel placeholder layout");

void fn_71001c4418(void* self, const s32* value);
UnkLabelSource* fn_71011b9fac(UnkGuiRoot* root, const void* key);
void fn_71011baa38(UnkGuiRoot* root, char* source, UnkLabelViewport** out, s32 frame);
UnkLabelVec2 fn_7100b82b18(const UnkLabelSource* source, s32 frame);

// The label's depth: field_0x140, or field_0x13c while it is unset.
static inline s32 Depth(const UnkLabel* self) {
    s32 depth = self->field_0x140;
    if (depth == 0x7fffffff)
        depth = self->field_0x13c;
    return depth;
}

static inline void SetViewPos(UnkLabelViewport* viewport, const CVector2D& pos) {
    UnkMinimapView* v = viewport->view;
    v->pos.x = pos.x;
    v->pos.y = pos.y;
    fn_71001c36c0(v, &v->pos, &v->size);
    viewport->node.flag_0xfb = true;
}
static inline void SetViewSize(UnkLabelViewport* viewport, const CVector2D& size) {
    UnkMinimapView* v = viewport->view;
    v->size.x = size.x;
    v->size.y = size.y;
    fn_71001c36c0(v, &v->pos, &v->size);
    viewport->node.flag_0xfb = true;
}

// Refreshes every entry of every item of the label's groups for the current frame (nothing without
// one): an item flagged at +0x10 first updates its object at +0x68; an unresolved entry gets its
// viewport from the GUI root, placed, sized and scaled; then each viewport is shown at the label's
// depth (field_0x140, else field_0x13c) less one. The current frame is the label's derived state's
// (fn_71011cde14, with the label seen as the display object it is).
void fn_71011ee5a8(UnkLabel* self) {
    UnkGuiObjectState* composition = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
    if (composition->frame == 0x7fffffff)
        return;
    for (UnkLabelGroupNode* g = self->groups; g; g = g->next) {
        for (UnkLabelItemNode* n = g->group->items; n; n = n->next) {
            UnkLabelItem* item = n->item;
            if (item->field_0x10 && *item->field_0x10 != 0) {
                UnkLabelItemSub* sub = item->field_0x68;
                fn_71001c4418(sub->field_0x130, item->field_0x10);
                sub->field_0x11b = true;
            }
            for (u64 i = 0; i < item->count; ++i) {
                UnkLabelEntry* e = &item->entries[i];
                if (e->field_0x0 == 0) {
                    if (UnkLabelSource* source = fn_71011b9fac(g_unk_717271e8f8, e->key)) {
                        fn_71011baa38(g_unk_717271e8f8, source->field_0x18 + 8, &e->viewport, composition->frame);
                        if (e->viewport) {
                            SetViewPos(e->viewport, source->pos);
                            SetViewSize(e->viewport, source->size);
                            s32 frame = composition->frame;
                            UnkLabelVec2 scaled = fn_71011b8160(frame, fn_7100b82b18(source, frame));
                            UnkLabelVec2 scale = fn_71011b7f48(composition->frame, scaled);
                            // The scale is passed by value: a copy made after the viewport is read.
                            UnkLabelViewport* viewport = e->viewport;
                            UnkLabelVec2 arg = scale;
                            fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(&viewport->node),
                                          reinterpret_cast<const CVector2D*>(&arg));
                            fn_71001c3770(e->viewport->view, true);
                        }
                    }
                }
                if (e->viewport) {
                    fn_710020ac34(&e->viewport->node, false);
                    e->frame = composition->frame;
                    UnkLabelViewport* viewport = e->viewport;
                    s32 depth = Depth(self) - 1;
                    fn_710020ad90(&viewport->node, depth);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------
// The label's text layout (fn_71011ed1e8): lays the shown text (+0x218) out into lines of runs
// (the groups and items fn_71011ee5a8 refreshes). Each run is a text object of the GUI root with
// its own text (the characters with their spacing commands) and the glyphs of the tags it shows;
// a run ends at a line break ('\n', or '|' when +0x2fc is set) or where the text no longer fits
// the width (the label's +0x2a8, or its derived width when +0x185 is clear and that is smaller),
// the text then going back to the last space. The text's own syntax: "{cN}" sets the run's colour
// digit, a byte 1-9 is a control character passed on with the next character, and "@...]" (with
// +0x2fc set) is a tag: "@N<hex>]" a glyph of font N by code, "@name:scale]" a sprite looked up by
// name (with the label's font name appended first, then as written). Names and layouts are
// placeholders.

#include <new>

#include "base/Char32.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A text of 32-bit characters (game/UnkGuiLabel.h's UnkGuiText): the characters at +0x8, null when
// empty, and their count at +0x10.
void fn_7100066a50(void* self, const char32_t* text);
void fn_7100066e44(void* text);
struct UnkWideStr_0x20;
void fn_7100066b38(UnkWideStr_0x20* self, const char* str);
struct UnkLayoutText {
    u8 unk_0x0[0x8];
    const char32_t* chars;
    u32 count;
    u8 unk_0x14[0xc];

    explicit UnkLayoutText(const char32_t* text) { fn_7100066a50(this, text); }
    ~UnkLayoutText() { fn_7100066e44(this); }
};
static_assert(sizeof(UnkLayoutText) == 0x20, "UnkLayoutText placeholder layout");

// A spacing command appended to a text: kind 9 inserts `count` units of space.
struct UnkTextCommand {
    u32 kind;
    u32 count;
    u32 field_0x8;
};

// The text functions: assignment from a text and from characters, appending a character and a
// command, a substring (from `start`, `count` characters, -1 for the rest) and the conversion to a
// string.
void fn_7100066d4c(UnkLayoutText* self, const UnkLayoutText* other);
void fn_7100066af0(UnkLayoutText* self, const char32_t* text);
void fn_7100067430(UnkLayoutText* self, u32 c);
void fn_71000673e8(UnkLayoutText* self, const UnkTextCommand* command);
UnkLayoutText fn_7100067484(const UnkLayoutText* self, u32 start, u32 count);
CRntString fn_710006c080(const char32_t* text);

// A string id built from a string out of line (fn_7100000850), released as CStrId.
struct UnkLabelStrId {
    union {
        CStrId id;
    };

    UnkLabelStrId() {}
    void Build(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkLabelStrId() { id.~CStrId(); }
};

// A glyph id: built out of line from a string (fn_71000003d4), copied and assigned as CStrId is,
// released in two steps (the original reloads the id and its count after the decrement).
struct UnkGlyphId {
    union {
        CStrId id;
    };

    UnkGlyphId(const char* str, bool flag) { new (&id) CStrId(str, flag); }
    UnkGlyphId(const UnkGlyphId& other) { new (&id) CStrId(other.id); }
    ~UnkGlyphId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id.data && id.data != &pool->empty) {
                --id.data->ref_count;
                if (id.data->ref_count == 0) {
                    fn_71000803a4(pool, id.data);
                    pool = g_unk_7101d47108;
                }
            }
            id.data = &pool->empty;
        }
    }
};

void fn_71011bab80(UnkGuiRoot* root, void* object, s32 frame);

// A glyph a run shows (0x30 bytes; CLabel.cpp's UnkLabelEntry): a font glyph (kind 1, by font and
// code) or a sprite (kind 0, by id), 0x7fffffff while it is none, with where it is drawn and the
// object made for it in a frame.
struct UnkLabelGlyph {
    u32 kind;
    UnkGlyphId id;
    u8 font;
    u16 code;
    f32 x;
    f32 y;
    u32 field_0x1c;
    f32 scale;
    s32 frame;
    void* object;

    UnkLabelGlyph(f32 x, f32 y)
        : kind(0x7fffffff), id("", true), font(0), code(0), x(x), y(y), field_0x1c(0), scale(1.0f),
          frame(0x7fffffff), object(nullptr) {}
    UnkLabelGlyph(const UnkLabelGlyph& other) = default;
    ~UnkLabelGlyph() {
        if (object && frame != 0x7fffffff) {
            fn_71011bab80(g_unk_717271e8f8, object, frame);
            object = nullptr;
        }
    }
};
static_assert(sizeof(UnkLabelGlyph) == 0x30, "UnkLabelGlyph placeholder layout");

using UnkGlyphVector = base::UnkVector<UnkLabelGlyph>;
// Reserves `capacity` glyphs; assigns another vector's glyphs.
void fn_71011f05d4(UnkGlyphVector* self, u32 capacity);
void fn_71011ee3b4(UnkGlyphVector* self, const UnkGlyphVector* other);

// The GUI root's text object a run draws with (its +0x130 is the text node).
struct UnkLabelTextNode {
    u8 unk_0x0[0xe4];
    u32 color;  // the colour digit ('0' to '9')
};
struct UnkLabelTextObject {
    u8 unk_0x0[0x11b];
    bool field_0x11b;
    u8 unk_0x11c[0x14];
    UnkLabelTextNode* node;
    void* font;
};
void fn_71011bab10(UnkGuiRoot* root, UnkLabelTextObject** out, s32 frame);
void fn_71001c3ec0(UnkLabelTextNode* node, void* font_data);

// A run (0x70 bytes; CLabel.cpp's UnkLabelItem).
struct UnkLabelRun {
    s32 frame;
    UnkLayoutText text;
    f32 x;           // the pen, from the run's start
    f32 field_0x2c;  // where the run starts
    f32 y;
    u32 field_0x34;
    u32 field_0x38;
    u8 unk_0x3c[0x4];
    UnkGlyphVector glyphs;
    UnkLabelTextObject* object;
};
static_assert(sizeof(UnkLabelRun) == 0x70, "UnkLabelRun placeholder layout");

struct UnkLabelLinesTag;
struct UnkLabelRunsTag;

// A line (0x38 bytes; CLabel.cpp's UnkLabelGroup): its size and its runs.
extern char g_unk_7101cb65c8[];  // the run list's vtable
struct UnkLabelLine {
    f32 width;
    f32 height;
    base::UnkSList<UnkLabelRunsTag> runs;
};
static_assert(sizeof(UnkLabelLine) == 0x38, "UnkLabelLine placeholder layout");

// The label as its layout sees it (UnkLabel above, src/game/GUI/CLabel/fn_71011ebdb4.cpp's constructor).
struct UnkLabelLayout {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8(s32 frame);  // runs after a layout when +0x185 is set

    u8 unk_0x8[0x17d];
    bool field_0x185;  // the label keeps its own width (+0x2a8) rather than its derived one
    u8 unk_0x186[0x92];
    UnkLayoutText text;  // +0x218: the text shown
    u8 unk_0x238[0x20];
    bool dirty;  // +0x258
    u8 unk_0x259[0x7];
    CStrId font_id;            // +0x260
    CRntString font_name;      // +0x268: appended to sprite names
    void* font;                // +0x298
    u8 unk_0x2a0[0x8];
    f32 max_width;             // +0x2a8
    u32 max_runs;              // +0x2ac: the runs a line takes
    s32 space;                 // +0x2b0: the line advance's height (the font's space height when negative)
    u8 unk_0x2b4[0xc];
    base::UnkSList<UnkLabelLinesTag> lines;  // +0x2c0
    u32 field_0x2f0;
    u8 unk_0x2f4[0x8];
    bool tags;                 // +0x2fc: '@' tags and '|' line breaks are read
    u8 unk_0x2fd[0xb];
    s32 fixed_advance;         // +0x308: every character takes this much room when positive
    u32 field_0x30c;           // cleared with +0x310 by one 8-byte store at the layout's start
    u32 field_0x310;
};
static_assert(__builtin_offsetof(UnkLabelLayout, text) == 0x218, "UnkLabelLayout placeholder layout");
static_assert(__builtin_offsetof(UnkLabelLayout, lines) == 0x2c0, "UnkLabelLayout placeholder layout");
static_assert(__builtin_offsetof(UnkLabelLayout, fixed_advance) == 0x308, "UnkLabelLayout placeholder layout");
static_assert(__builtin_offsetof(UnkLabelLayout, field_0x310) == 0x310, "UnkLabelLayout placeholder layout");

// The derived state's width (UnkGuiObjectState's +0x14).
struct UnkLabelStateWidth {
    u8 unk_0x0[0x14];
    f32 width;
};

void fn_71011ec314(UnkLabelLayout* self);
// The font the root has for an id, and its name.
void* fn_71011b882c(UnkGuiRoot* root, const CStrId* id);
CRntString fn_71011b8790(UnkGuiRoot* root, const CStrId* id);
// A character's advance and height in a font.
u32 fn_7100147960(void* font, u32 c);
u32 fn_71001479dc(void* font, u32 c);
// Scales a size into the frame's units.
UnkLabelVec2 fn_71011b7e7c(UnkGuiRoot* root, s32 frame, const CVector2D* size);
UnkLabelVec2 fn_71011b800c(UnkGuiRoot* root, s32 frame, const UnkLabelVec2* size);
// A font glyph's metrics (the advance at +0x8); false when the font has no such glyph.
struct UnkGlyphMetrics {
    u8 unk_0x0[0x8];
    f32 advance;
    u8 unk_0xc[0x14];
};
bool fn_71011b89dc(UnkGuiRoot* root, u8 font, const CStrId* font_id, u32 code, UnkGlyphMetrics* out);

// The language singleton (src/game/UnkLanguage.cpp) and its current language.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
s32 fn_710119c9f4(UnkLanguage* language);
static inline UnkLanguage* LabelLanguage() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

// Appends `count` units of space (the command's other fields as the default command, {32, 32, 0}).
static inline void AppendSpace(UnkLayoutText* text, u32 count) {
    UnkTextCommand command = {9, count & 0xff, 0};
    fn_71000673e8(text, &command);
}

// Appends a glyph to a run (the array grown by its step first).
static inline void AddGlyph(UnkGlyphVector* glyphs, const UnkLabelGlyph& glyph) {
    u32 n = glyphs->count;
    u32 capacity = glyphs->capacity;
    if (capacity <= n && glyphs->step != 0)
        capacity = (n / glyphs->step + 1) * glyphs->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_71011f05d4(glyphs, capacity);
    new (&glyphs->data[glyphs->count]) UnkLabelGlyph(glyph);
    ++glyphs->count;
}

// NON_MATCHING(minor): written from the original's control flow; the registers, the stack slots and
// the block order differ throughout, and one call site of fn_710119c9f4 replaces the original's two
// (the same calls in the same order). As in the original, a font glyph tag without hex digits leaves
// its code uninitialised.
void fn_71011ed1e8(UnkLabelLayout* self, bool only_if_shown) {
    UnkGuiRoot** root = &g_unk_717271e8f8;
    UnkGuiDisplayObject* display = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011ec314(self);
    UnkGuiObjectState* state = fn_71011cde14(display);
    if (only_if_shown && !(state->field_0x3c && state->field_0x3e)) {
        self->dirty = false;
        return;
    }
    void* font = self->font;
    if (!font) {
        if (fn_71011cde14(display)->frame != 0x7fffffff && self->font_id.data != &g_unk_7101d47108->empty) {
            font = fn_71011b882c(*root, &self->font_id);
            self->font = font;
        } else {
            font = self->font;
        }
        if (!font) {
            self->dirty = false;
            return;
        }
    }
    if (!self->text.chars || self->text.chars[0] == 0) {
        self->dirty = false;
        return;
    }
    self->field_0x30c = 0;
    self->field_0x310 = 0xffffffff;
    if (state->frame == 0x7fffffff)
        return;

    f32 limit = self->max_width;
    if (!self->field_0x185) {
        f32 width = reinterpret_cast<UnkLabelStateWidth*>(fn_71011cde14(display))->width;
        if (width < limit && width > 0.0f)
            limit = width;
    }
    self->field_0x2f0 = 0;
    CRntString name(self->font_name);
    if (!name.data || name.data[0] == 0) {
        CRntString found = fn_71011b8790(*root, &self->font_id);
        fn_710007eaa0(&name, &found);
    }
    CVector2D unit = {1.0f, 1.0f};
    UnkLabelVec2 scale = fn_71011b7e7c(*root, state->frame, &unit);
    f32 space = self->space < 0 ? (f32)fn_71001479dc(font, ' ') : (f32)self->space;
    CVector2D line_size = {0.0f, space};
    UnkLabelVec2 advance = fn_71011b7e7c(*root, state->frame, &line_size);

    f32 y = 0.0f;
    f32 run_start = 0.0f;
    UnkLabelLine* line = nullptr;
    u32 i = 0;
    bool new_line = true;
    u32 color = '0';
    for (;;) {
        if (i >= (self->text.chars ? self->text.count : 0))
            break;
        if (new_line || line->runs.count >= self->max_runs) {
            line = static_cast<UnkLabelLine*>(fn_71000839a0(sizeof(UnkLabelLine), 0xc));
            line->runs.count = 0;
            line->runs.head = nullptr;
            line->runs.tail = nullptr;
            line->width = 0.0f;
            line->height = 0.0f;
            line->runs.field_0x0 = reinterpret_cast<u64>(g_unk_7101cb65c8 + 0x10);
            line->runs.allocator = fn_7100083990();
            line->runs.flags = 0x13;
            self->lines.PushBack(reinterpret_cast<u64>(line));
            y = 0.0f;
            run_start = 0.0f;
        }

        // The run: its text object takes the label's font, and the line's first run the colour '0'.
        auto* run = static_cast<UnkLabelRun*>(fn_71000839a0(sizeof(UnkLabelRun), 0xc));
        s32 frame = state->frame;
        run->frame = frame;
        fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(&run->text), "");
        run->x = 0.0f;
        run->field_0x2c = 0.0f;
        run->glyphs.count = 0;
        run->glyphs.capacity = 0;
        run->glyphs.data = nullptr;
        run->y = 0.0f;
        run->field_0x34 = 0;
        run->field_0x38 = 0;
        run->glyphs.step = 10;
        run->glyphs.field_0x14 = true;
        run->glyphs.flags = 0xc;
        run->glyphs.allocator = fn_7100083990();
        fn_71011f05d4(&run->glyphs, 10);
        run->object = nullptr;
        fn_71011bab10(*root, &run->object, frame);
        run->field_0x2c = run_start;
        run->y = y;
        UnkLabelTextObject* object = run->object;
        object->font = font;
        fn_71001c3ec0(object->node, static_cast<char*>(font) + 0x18);
        object->field_0x11b = true;
        line->runs.PushBack(reinterpret_cast<u64>(run));
        if (line->runs.count == 1)
            color = '0';
        run->object->node->color = color;

        UnkLayoutText tag_name(U"");
        UnkLayoutText tag_unused(U"");
        UnkLayoutText tag_value(U"");
        UnkLayoutText saved_text(U"");
        UnkGlyphVector saved_glyphs(0xc);
        fn_71011f05d4(&saved_glyphs, 10);
        f32 saved_x = 0.0f;
        f32 pen = run_start;
        bool in_tag = false;
        bool after_colon = false;
        u32 control = 0;
        s32 last_break = -1;
        for (;;) {
            const char32_t* chars = self->text.chars;
            if (i >= (chars ? self->text.count : 0)) {
                new_line = false;
                break;
            }
            const char32_t* p = chars ? chars : U"";
            u32 c = p[i];
            if (c == '{') {
                u32 n = chars ? self->text.count : 0;
                if (i + 4 <= n && p[i + 3] == '}') {
                    u32 c1 = p[i + 1];
                    if ((c1 | 0x20) == 'c') {
                        u32 c2 = p[i + 2];
                        if (c2 - '0' < 10) {
                            CVector2D width = {0.0f, 0.0f};
                            width.x += (f32)fn_7100147960(self->font, '{');
                            width.x += (f32)fn_7100147960(self->font, c1);
                            width.x += (f32)fn_7100147960(self->font, c2);
                            width.x += (f32)fn_7100147960(self->font, '}');
                            UnkLabelVec2 scaled = fn_71011b7e7c(*root, state->frame, &width);
                            color = c2;
                            run->x = run->x - scaled.x;
                        }
                    }
                }
            }

            bool glyph = false;
            if (in_tag) {
                switch (c) {
                case ':':
                    after_colon = true;
                    ++i;
                    continue;
                case '[':
                    ++i;
                    continue;
                case '@':
                    glyph = true;
                    break;
                case ']':
                    break;
                default:
                    if (c == '|') {
                        ++i;
                        new_line = true;
                        goto end_run;
                    }
                    if (after_colon)
                        fn_7100067430(&tag_value, c);
                    else
                        fn_7100067430(&tag_name, c);
                    ++i;
                    continue;
                }
            } else {
                bool special = control == 0 && self->tags;
                if (c == '|' && special) {
                    ++i;
                    new_line = false;
                    goto end_run;
                }
                if (c == '@' && special) {
                    control = 0;
                    in_tag = true;
                    ++i;
                    continue;
                }
                if (control == 0 && c == '\n') {
                    ++i;
                    new_line = false;
                    goto end_run;
                }
                if (control == 0 && c - 1 < 9) {
                    in_tag = false;
                    control = c;
                    ++i;
                    continue;
                }
                glyph = true;
            }

            if (glyph) {
                // A character: its advance (the fixed one when larger) scaled into the frame.
                f32 char_advance = (f32)fn_7100147960(font, c);
                CVector2D size = {char_advance, (f32)fn_71001479dc(font, c)};
                if (control == 0 && self->fixed_advance >= 1) {
                    s32 a = (s32)char_advance;
                    size.x = (f32)(self->fixed_advance > a ? self->fixed_advance : a);
                }
                UnkLabelVec2 scaled = fn_71011b7e7c(*root, state->frame, &size);
                f32 x = run->x + scaled.x;
                if (x > limit && ((run->text.chars && run->text.chars[0] != 0) || run->glyphs.count != 0)) {
                    // It doesn't fit: back to the last space (unless the language breaks anywhere), else
                    // the character starts the next run (with its control character).
                    s32 language = fn_710119c9f4(LabelLanguage());
                    if (last_break >= 0 && !((u32)(language - 10) < 4)) {
                        run->x = saved_x;
                        fn_7100066d4c(&run->text, &saved_text);
                        fn_71011ee3b4(&run->glyphs, &saved_glyphs);
                        i = (u32)last_break + 1;
                        last_break = -1;
                    } else {
                        if (i != 0) {
                            u32 j = i - 1;
                            if (j == 0) {
                                i = 0;
                            } else {
                                const char32_t* q = self->text.chars ? self->text.chars : U"";
                                i = q[j] - 1 < 9 ? i - 2 : j;
                            }
                        }
                        ++i;
                    }
                    in_tag = false;
                    new_line = false;
                    goto end_run;
                }
                s32 pad_before = 0;
                s32 pad_after = 0;
                if (self->fixed_advance >= 1) {
                    f32 d = (f32)self->fixed_advance - char_advance;
                    s32 pad = (s32)(d + (d > 0.0f ? 0.5f : -0.5f));
                    if (pad >= 1) {
                        pad_before = (s32)((f32)pad * 0.5f);
                        pad_after = pad - pad_before;
                    }
                }
                if (c == ' ') {
                    fn_7100066d4c(&saved_text, &run->text);
                    saved_x = run->x;
                    fn_71011ee3b4(&saved_glyphs, &run->glyphs);
                    last_break = (s32)i;
                }
                if (pad_before >= 1)
                    AppendSpace(&run->text, (u32)pad_before);
                if (control != 0)
                    fn_7100067430(&run->text, control);
                fn_7100067430(&run->text, c);
                // The space after takes the count of the space before, as in the original.
                if (pad_after >= 1)
                    AppendSpace(&run->text, (u32)pad_before);
                run->x = x;
                pen += scaled.x;
                control = 0;
                in_tag = false;
                ++i;
                continue;
            }

            // ']': the end of a tag.
            {
                bool done = false;
                s32 next_break = last_break;
                f32 next_pen = pen;
                if (tag_name.chars && tag_name.chars[0] != 0) {
                    UnkLabelGlyph g(pen, y);
                    u32 width = 0;
                    bool place = true;
                    if (tag_name.chars[0] - '0' < 10) {
                        // A font glyph: the font's digit and the code in hex.
                        g.kind = 1;
                        g.font = (u8)tag_name.chars[0] - '0';
                        g.scale = -1.0f;
                        u32 code;
                        {
                            UnkLayoutText hex = fn_7100067484(&tag_name, 1, (u32)-1);
                            char32_scan_hex(hex.chars ? hex.chars : U"", &code);
                        }
                        UnkGlyphMetrics metrics = {};
                        g.code = (u16)code;
                        if (fn_71011b89dc(*root, g.font, &self->font_id, code, &metrics))
                            width = (u32)(s32)(metrics.advance + 0.5f);
                    } else {
                        // A sprite: by its name with the label's font name appended, else by its name.
                        g.kind = 0;
                        {
                            CRntString str = fn_710006c080(tag_name.chars);
                            UnkLabelStrId id;
                            id.Build(str);
                            g.id.id = static_cast<CStrId&&>(id.id);
                        }
                        g.scale = -1.0f;
                        if (tag_value.chars && tag_value.chars[0] != 0)
                            char32_scan_f32(tag_value.chars, &g.scale);
                        UnkLabelSource* sprite = nullptr;
                        if (name.data && name.data[0] != 0) {
                            UnkLabelStrId full;
                            {
                                CRntString a = fn_710006c080(tag_name.chars);
                                CRntString b = fn_710007eee0(&a, &name);
                                full.Build(b);
                            }
                            sprite = fn_71011b9fac(*root, &full.id);
                            if (sprite)
                                g.id.id = full.id;
                        }
                        if (!sprite)
                            sprite = fn_71011b9fac(*root, &g.id.id);
                        if (sprite) {
                            UnkLabelVec2 sprite_size = fn_7100b82b18(sprite, state->frame);
                            UnkLabelVec2 scaled = fn_71011b800c(*root, state->frame, &sprite_size);
                            s32 w = (s32)scaled.x;
                            width = (u32)(w < 0xff ? w : 0xff);
                        } else {
                            g.kind = 0x7fffffff;
                            place = false;
                        }
                    }
                    if (place) {
                        u32 count = width & 0xff;
                        f32 x = run->x + scale.x * (f32)(s32)count;
                        if (x >= limit && ((run->text.chars && run->text.chars[0] != 0) || run->glyphs.count != 0)) {
                            if (last_break >= 0) {
                                run->x = saved_x;
                                fn_7100066d4c(&run->text, &saved_text);
                                fn_71011ee3b4(&run->glyphs, &saved_glyphs);
                                i = (u32)last_break;
                                next_break = -1;
                            } else if (i != 0) {
                                // Back to before the tag's '@'.
                                const char32_t* q = self->text.chars ? self->text.chars : U"";
                                do {
                                    u32 c2 = q[i];
                                    --i;
                                    if (c2 == '@')
                                        break;
                                } while (i != 0);
                            }
                            done = true;
                        } else {
                            AddGlyph(&run->glyphs, g);
                            AppendSpace(&run->text, count);
                            run->x = x;
                            next_pen = x;
                        }
                    }
                }
                fn_7100066af0(&tag_unused, U"");
                fn_7100066af0(&tag_value, U"");
                fn_7100066af0(&tag_name, U"");
                in_tag = false;
                after_colon = false;
                pen = next_pen;
                last_break = next_break;
                ++i;
                if (done) {
                    new_line = false;
                    break;
                }
            }
        }
    end_run:
        y += advance.y;
        f32 advance_x = advance.x;
        line->width = line->width > run->x ? line->width : run->x;
        line->width = advance_x + line->width;
        line->height = advance.y + line->height;
        saved_glyphs.Free();
        run_start += advance_x;
    }
    fn_71011ee5a8(reinterpret_cast<UnkLabel*>(self));
    if (self->field_0x185)
        self->vf_0xa8(state->frame);
    self->dirty = false;
}

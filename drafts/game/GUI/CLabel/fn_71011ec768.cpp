// From unit 2752 of tools/tu_map.py (GUI::CLabel): places the label's laid-out runs and their glyphs
// for the current frame, shows the runs of the current page and reveals their characters up to the
// label's limit. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "engine/scene/UnkToggle.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU135.h"
#include "game/UnkTU1722.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU287.h"
#include "game/UnkXformNode.h"
#include "types.h"

// From src/game/UnkGuiItemState.cpp (the same definitions).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
};

struct UnkGuiAnchor {
    u32 horizontal;
    u32 vertical;
    UnkGuiPos offset;
};

struct UnkGuiColor {
    f32 r, g, b, a;
};

struct UnkGuiScale {
    f32 x, y;

    UnkGuiScale operator*(const UnkGuiScale& o) const { return {x * o.x, y * o.y}; }
};

struct UnkGuiItemState {
    UnkGuiAnchor anchor;  // the offset is the position
    f32 angle;
    f32 width;   // the area a child is anchored in (UnkGuiArea)
    f32 height;
    UnkGuiScale scale;
    f32 depth;
    UnkGuiColor color;
    s32 field_0x38;  // INT32_MAX: the parent's
    bool field_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
};
static_assert(sizeof(UnkGuiItemState) == 0x40, "UnkGuiItemState placeholder layout");

// From src/game/CMinimapManager.cpp (the same definition).
struct UnkMinimapVec3 {
    f32 x;
    f32 y;
    f32 z;
    UnkMinimapVec3() {}
    UnkMinimapVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkMinimapVec3& operator=(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        return *this;
    }
    UnkMinimapVec3 operator*(f32 s) const { return UnkMinimapVec3(x * s, y * s, z * s); }
    UnkMinimapVec3(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
    }
};

struct UnkLabelFontView_b52;
struct UnkOwner_71011b8724;
struct UnkFrameOwner;
struct UnkModeOwner257;
struct UnkColorLayer224;
struct UnkGuiListOwner_b49;
struct UnkXformNodeNegZ;
struct UnkFadeLayer;

void fn_71011eec70(UnkGuiDisplayObject* object);
s32 CLabel_GetVisibleLineCount(UnkLabelFontView_b52* self);
void* fn_71011b8724(UnkOwner_71011b8724* self, const base::global::CStrId* key);
u32 fn_7100147960(UnkFrameOwner* self, u32 c);
UnkVec2Result fn_71011cf1b4(UnkGuiDisplayObject* self, const UnkVec2Result& point);
void fn_710020c128(UnkToggle* self, bool on);
u32 fn_710021acb0(u32 color);
void fn_71001c465c(UnkModeOwner257* self, s32 mode);
UnkGuiPos fn_71011f61bc(const UnkGuiItemState* state, const UnkGuiPos* pos);
const UnkGuiScale* fn_71011b7e74(UnkGuiRoot* root);
void fn_710020bbe4(void* position, const f32* value);
void SetColorAndCacheRGBA8Bytes(UnkColorLayer224* self, const base::color::CColor4F* color);
void* CGUIManager_FindInEntryListByKey(UnkGuiListOwner_b49* self, u64 key);
void fn_7100b1c56c(const UnkGuiItemState* parent, UnkGuiItemState* out, const UnkGuiItemState* child, u32 mode,
                   u32 changes);
void fn_710020a6e8(UnkXformNodeNegZ* self, const UnkVec3f* v);
void SetFadeLayerColor(UnkFadeLayer* layer, const base::color::CColor4F* color);

extern f32 g_unk_710164d1c8;  // the colour scale

// A 4-byte value copied whole (as bytes).
struct UnkStruct_0x4 {
    u8 bytes[4];
};

// The layer a run's text is drawn with: its colour mode, the characters it shows and its style.
struct UnkLabelRunLayer {
    u8 pad_0x0[0x14c];
    u32 field_0x14c;
    u8 pad_0x150[0x10];
    s32 shown;  // +0x160: the characters drawn
    u8 pad_0x164[0x50];
    u8 field_0x1b4;
    u8 pad_0x1b5[3];
    UnkStruct_0x4 field_0x1b8;
    UnkStruct_0x4 field_0x1bc;
};
static_assert(__builtin_offsetof(UnkLabelRunLayer, field_0x1bc) == 0x1bc, "UnkLabelRunLayer placeholder layout");

// A run's text node: its layer, size and length.
struct UnkLabelRunText {
    u8 pad_0x0[0x78];
    UnkLabelRunLayer* layer;
    u8 pad_0x80[0x14];
    UnkGuiScale size;  // +0x94
    u8 pad_0x9c[0x40];
    s32 length;  // +0xdc
};
static_assert(__builtin_offsetof(UnkLabelRunText, length) == 0xdc, "UnkLabelRunText placeholder layout");

// The GUI root's text object a run draws with.
struct UnkLabelRunObject {
    u8 pad_0x0[0x20];
    UnkToggle node;
    UnkLabelRunText* text;  // +0x130
};
static_assert(__builtin_offsetof(UnkLabelRunObject, text) == 0x130, "UnkLabelRunObject placeholder layout");

// The object a glyph is drawn with.
struct UnkLabelGlyphLayerHolder {
    u8 pad_0x0[0x78];
    UnkFadeLayer* layer;
};
struct UnkLabelGlyphObject {
    u8 pad_0x0[0x18];
    UnkToggle node;
    u8 pad_0x128[0x44];
    UnkLabelVec2 size;  // +0x16c
    u8 pad_0x174[0x4];
    UnkLabelGlyphLayerHolder* holder;  // +0x178
};
static_assert(__builtin_offsetof(UnkLabelGlyphObject, holder) == 0x178, "UnkLabelGlyphObject placeholder layout");

// A glyph of a run (0x30 bytes): a font glyph (kind 1) or a sprite, found by its id.
struct UnkLabelPlacedGlyph {
    u32 kind;
    u8 pad_0x4[0x4];
    u8 id[0x8];
    u8 pad_0x10[0x4];
    f32 x;
    f32 y;
    f32 field_0x1c;
    f32 scale;  // negative: the sprite keeps its own size
    s32 frame;
    UnkLabelGlyphObject* object;
};
static_assert(sizeof(UnkLabelPlacedGlyph) == 0x30, "UnkLabelPlacedGlyph placeholder layout");

// A run of a line (0x70 bytes).
struct UnkLabelPlacedRun {
    s32 frame;
    u8 pad_0x4[0x28];
    UnkGuiPos pos;  // +0x2c
    f32 field_0x34;
    f32 field_0x38;
    u8 pad_0x3c[0x4];
    UnkLabelPlacedGlyph* glyphs;  // +0x40
    u32 glyph_count;              // +0x48
    u8 pad_0x4c[0x1c];
    UnkLabelRunObject* object;  // +0x68
};
static_assert(sizeof(UnkLabelPlacedRun) == 0x70, "UnkLabelPlacedRun placeholder layout");

struct UnkLabelRunNode {
    UnkLabelPlacedRun* run;
    UnkLabelRunNode* next;
};
struct UnkLabelPlacedLine {
    u8 pad_0x0[0x18];
    UnkLabelRunNode* runs;
};
struct UnkLabelLineNode {
    UnkLabelPlacedLine* line;
    UnkLabelLineNode* next;
};

// The skin colour a label may take.
struct UnkLabelSkin {
    u8 pad_0x0[0x38];
    u32 color;
};

// The label as it is placed here.
struct UnkLabelPlaced : UnkGuiDisplayObject {
    u8 pad_0x198[0xc8];
    base::global::CStrId font_id;  // +0x260
    u8 pad_0x268[0x30];
    UnkFrameOwner* font;  // +0x298
    u8 pad_0x2a0[0x10];
    s32 space;  // +0x2b0: the line height (the font's space height when negative)
    u8 pad_0x2b4[0x4];
    s32 mode;  // +0x2b8
    u8 pad_0x2bc[0xc];
    u32 line_count;          // +0x2c8
    u8 pad_0x2cc[0x4];
    UnkLabelLineNode* lines;  // +0x2d0
    u8 pad_0x2d8[0x18];
    u32 current_line;  // +0x2f0
    bool skin_color;   // +0x2f4
    u8 pad_0x2f5[0x3];
    u32 color;  // +0x2f8
    u8 pad_0x2fc[0x4];
    u32 first_run;  // +0x300: the first run of the page shown
    u8 pad_0x304[0x8];
    s32 shown;   // +0x30c: the characters shown
    s32 reveal;  // +0x310: the characters to show, all when negative
    u8 pad_0x314[0x1c];
    u8 field_0x330;
    u8 pad_0x331[0x3];
    UnkStruct_0x4 field_0x334;
    UnkStruct_0x4 field_0x338;
};
static_assert(__builtin_offsetof(UnkLabelPlaced, font_id) == 0x260, "UnkLabelPlaced placeholder layout");
static_assert(__builtin_offsetof(UnkLabelPlaced, lines) == 0x2d0, "UnkLabelPlaced placeholder layout");
static_assert(__builtin_offsetof(UnkLabelPlaced, field_0x338) == 0x338, "UnkLabelPlaced placeholder layout");

static inline UnkGuiItemState* State(UnkLabelPlaced* self) {
    return reinterpret_cast<UnkGuiItemState*>(fn_71011cde14(self));
}

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

// A size scaled into the frame's units.
static inline UnkLabelVec2 FrameSize(UnkGuiRoot* root, s32 frame, f32 width, f32 height) {
    base::math::CVector2D size = {width, height};
    return fn_71011b7e7c(root, frame, &size);
}

// Whether a state is shown and not fully transparent.
static inline bool Shown(const UnkGuiItemState& state) {
    return ((state.field_0x3c != 0) & (state.field_0x3d != 0)) && state.color.a > 0.0f;
}

// Where a run starts in the label's frame: its place turned and scaled by the label's state, offset
// by the label's position and the run's own offsets.
static inline UnkLabelVec2 RunPosition(const UnkGuiItemState* state, const UnkLabelPlacedRun* run, f32 y_offset) {
    UnkGuiPos at = fn_71011f61bc(state, &run->pos);
    return UnkLabelVec2(state->anchor.offset.x + at.x + run->field_0x34, y_offset + (state->anchor.offset.y + at.y));
}

// A point of the frame on screen.
static inline UnkLabelVec2 ToScreen(s32 frame, f32 x, f32 y) {
    UnkLabelVec2 point(x, y);
    return fn_71011b7ec0(frame, &point);
}

// Moves a node to a point at a depth.
static inline void SetPosition(UnkToggle* node, const UnkLabelVec2& point, f32 depth) {
    f32 position[3] = {point.x, point.y, depth};
    fn_710020bbe4(node, position);
}

// Turns a node by an angle (about z).
static inline void SetAngle(UnkToggle* node, f32 angle) {
    UnkVec3f angles(0.0f, 0.0f, -angle);
    fn_710020a6e8(reinterpret_cast<UnkXformNodeNegZ*>(node), &angles);
}

void fn_71011ec768(UnkLabelLayout* layout) {
    UnkLabelPlaced* self = reinterpret_cast<UnkLabelPlaced*>(layout);
    s32 reveal = self->reveal;
    UnkGuiItemState* state = State(self);
    if (state->field_0x38 != 0x7fffffff && self->line_count != 0) {
        fn_71011eec70(self);
        s32 page_runs = CLabel_GetVisibleLineCount(reinterpret_cast<UnkLabelFontView_b52*>(self));
        UnkGuiRoot** root = &g_unk_717271e8f8;
        if (!self->font && State(self)->field_0x38 != 0x7fffffff && self->font_id.data != &g_unk_7101d47108->empty)
            self->font = static_cast<UnkFrameOwner*>(CGUIManager_GetOrLoadCachedByPath(*root, &self->font_id));
        UnkFrameOwner* font = self->font;
        UnkLabelSkin* skin =
            static_cast<UnkLabelSkin*>(fn_71011b8724(reinterpret_cast<UnkOwner_71011b8724*>(*root), &self->font_id));
        f32 space_width = fn_7100147960(font, ' ');
        f32 line_height = self->space < 0 ? (f32)fn_71001479dc(font, ' ') : (f32)self->space;
        UnkLabelVec2 scaled = FrameSize(*root, state->field_0x38, space_width, line_height);
        UnkVec2Result space_local = fn_71011cf1b4(self, reinterpret_cast<const UnkVec2Result&>(scaled));

        self->shown = 0;
        bool placed = false;
        UnkLabelLineNode* line_node = self->lines;
        if (line_node) {
            s32 left = self->reveal;
            f32 color_scale = g_unk_710164d1c8;
            u32 line_index = 0;
            do {
                bool current = Shown(*state) && self->current_line == line_index;
                f32 y_offset = 0.0f;
                u32 run_index = 0;
                for (UnkLabelRunNode* run_node = line_node->line->runs; run_node;
                     run_node = run_node->next, ++run_index) {
                    UnkLabelPlacedRun* run = run_node->run;
                    if (run_index == self->first_run)
                        y_offset = -run->pos.y;
                    bool visible = current && !(run_index < self->first_run) && run_index < self->first_run + page_runs;
                    fn_710020c128(&run->object->node, visible);
                    if (!visible) {
                        for (u64 i = 0; i < run->glyph_count; ++i) {
                            if (UnkLabelGlyphObject* object = run->glyphs[i].object)
                                fn_710020ac34(&object->node, false);
                        }
                        continue;
                    }

                    s32 length = run->object->text->length;
                    self->shown += length;
                    if (reveal < 0) {
                        run->object->text->layer->shown = length;
                    } else if (left >= length) {
                        run->object->text->layer->shown = length;
                        left -= length;
                    } else {
                        run->object->text->layer->shown = left;
                        left = 0;
                    }
                    u32 default_color = self->skin_color ? skin->color : 0;
                    u32 color = self->color == 0 ? default_color : self->color;
                    UnkLabelRunLayer* layer = run->object->text->layer;
                    layer->field_0x14c = fn_710021acb0(color);
                    fn_71001c465c(reinterpret_cast<UnkModeOwner257*>(run->object->text), self->mode);
                    run->object->text->layer->field_0x1b4 = self->field_0x330;
                    run->object->text->layer->field_0x1bc = self->field_0x338;
                    run->object->text->layer->field_0x1b8 = self->field_0x334;

                    s32 frame = state->field_0x38;
                    UnkLabelVec2 framed = fn_71011b810c(frame, RunPosition(state, run, y_offset));
                    UnkLabelVec2 screen = ToScreen(state->field_0x38, framed.x + 0.0f, run->field_0x38 + framed.y);
                    run->object->text->size = state->scale * *fn_71011b7e74(g_unk_717271e8f8);
                    run->object->node.flag_0xfb = true;
                    SetPosition(&run->object->node, screen, state->depth);
                    {
                        base::color::CColor4F run_color(
                            NonNegative(state->color.r * color_scale), NonNegative(color_scale * state->color.g),
                            NonNegative(color_scale * state->color.b), NonNegative(color_scale * state->color.a));
                        SetColorAndCacheRGBA8Bytes(reinterpret_cast<UnkColorLayer224*>(run->object->text->layer), &run_color);
                    }

                    for (u64 i = 0; i < run->glyph_count; ++i) {
                        UnkLabelPlacedGlyph* glyph = &run->glyphs[i];
                        if (!glyph->object)
                            continue;
                        fn_710020ac34(&glyph->object->node, true);
                        UnkGuiItemState item = {{0, 0, UnkGuiPos(0.0f, 0.0f)},
                                                0.0f,
                                                1.0f,
                                                1.0f,
                                                {1.0f, 1.0f},
                                                0.0f,
                                                {1.0f, 1.0f, 1.0f, 1.0f},
                                                0x7fffffff,
                                                true,
                                                1,
                                                0};
                        f32 y = glyph->y / state->scale.y;
                        f32 x = glyph->x / state->scale.x;
                        void* source =
                            CGUIManager_FindInEntryListByKey(reinterpret_cast<UnkGuiListOwner_b49*>(g_unk_717271e8f8),
                                          reinterpret_cast<u64>(&glyph->id));
                        UnkLabelVec2 size(0.0f, 0.0f);
                        if (source) {
                            size = fn_7100b82b18(static_cast<const UnkLabelSource*>(source), state->field_0x38);
                            if (glyph->scale >= 0.0f) {
                                item.scale.x = glyph->scale * item.scale.x;
                                item.scale.y = glyph->scale * item.scale.y;
                            }
                        } else {
                            UnkLabelVec2 own = glyph->object->size;
                            size = fn_71011b7f88(state->field_0x38, own);
                            item.angle = 0.0f;
                        }
                        f32 width = item.scale.x * size.x;
                        f32 height = item.scale.y * size.y;
                        item.field_0x3c = true;
                        item.anchor.offset = UnkGuiPos(x + glyph->field_0x1c,
                                                       y + (space_local.y - height) * 0.5f + run->field_0x38);
                        fn_7100b1c56c(state, &item, &item, 0xffffffff, 0xffffffff);
                        UnkLabelVec2 center = fn_71011b810c(item.field_0x38,
                                                            reinterpret_cast<const UnkLabelVec2&>(item.anchor.offset));
                        center.x = width * 0.5f + center.x;
                        center.y = height * 0.5f + center.y;
                        if (glyph->kind == 1) {
                            UnkLabelVec2 down(0.0f, -1.0f);
                            UnkLabelVec2 drop = fn_71011b8078(state->field_0x38, down);
                            center = UnkLabelVec2(drop.x + center.x, drop.y + center.y);
                        }
                        UnkLabelVec2 glyph_screen = fn_71011b7ec0(item.field_0x38, &center);
                        glyph_screen.y += 0.008333f;
                        fn_710020a4c4(&glyph->object->node, UnkMinimapVec3(glyph_screen.x, glyph_screen.y, item.depth));
                        SetAngle(&glyph->object->node, item.angle);
                        if (source && glyph->scale >= 0.0f) {
                            UnkLabelVec2 sized(width, height);
                            sized = fn_71011b8160(item.field_0x38, sized);
                            UnkLabelVec2 scale = fn_71011b7f48(item.field_0x38, sized);
                            UnkLabelVec2 arg = scale;
                            fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(&glyph->object->node),
                                          reinterpret_cast<const base::math::CVector2D*>(&arg));
                        }
                        item.color.r *= 0.925f;
                        item.color.g *= 0.925f;
                        item.color.b *= 0.925f;
                        {
                            base::color::CColor4F glyph_color(
                                NonNegative(color_scale * item.color.r), NonNegative(color_scale * item.color.g),
                                NonNegative(color_scale * item.color.b), NonNegative(color_scale * item.color.a));
                            SetFadeLayerColor(glyph->object->holder->layer, &glyph_color);
                        }
                        fn_710020ac34(&glyph->object->node, item.field_0x3c);
                    }
                    placed = true;
                }
                line_node = line_node->next;
                ++line_index;
            } while (line_node);
        }
        self->vf_0x68();
        if (reveal < 0 && placed)
            self->reveal = self->shown;
    }
    self->vf_0x90();
}

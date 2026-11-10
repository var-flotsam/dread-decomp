// GUI::CText's unit (tools/tu_map.py unit 1724): the text's refresh, as GUI::CLabel's
// (src/game/GUI/CLabel.cpp). fn_7100b86f00 finds the font by its id once (fn_71011b882c) while the
// text is dirty, then rebuilds the text object (fn_7100b86de8) and lays it out (fn_7100b86b84).
// fn_7100b86de8 gives the old text object back to the GUI root, takes a new one for the frame,
// sets its font and value and keeps its size. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

struct UnkLabelTextNode {
    u8 unk_0x0[0x80];
    void* field_0x80;
};
struct UnkLabelTextObject {
    u32 bits;  // the frame it was taken for in bits 12 to 15
    u8 unk_0x4[0x117];
    bool field_0x11b;
    u8 unk_0x11c[0x14];
    UnkLabelTextNode* node;  // 0x130
    void* font;              // 0x138
};
static_assert(__builtin_offsetof(UnkLabelTextObject, font) == 0x138, "layout");

void* fn_71011b882c(UnkGuiRoot* root, const CStrId* id);
void fn_71011cee10(UnkGuiDisplayObject* self);
void fn_71011baba0(UnkGuiRoot* root, void* object, s32 frame);
void fn_71011bab10(UnkGuiRoot* root, UnkLabelTextObject** out, s32 frame);
void fn_71001c3ec0(UnkLabelTextNode* node, void* font_data);
void fn_71001c4418(void* self, const s32* value);
u32 fn_71001c4510(UnkLabelTextNode* node);
f32 fn_71001bd59c(void* self);

struct UnkTextValue_b0080 {
    u8 unk_0x0[0x8];
    const s32* value;
};

struct UnkText_b0080 {
    u8 unk_0x0[0x190];
    UnkLabelTextObject* object;  // 0x190
    f32 width;                   // 0x198
    f32 height;                  // 0x19c
    u8 unk_0x1a0[0x8];
    CStrId font_id;              // 0x1a8
    void* font;                  // 0x1b0
    u8 unk_0x1b8[0x60];
    UnkTextValue_b0080 value;    // 0x218
    u8 unk_0x228[0x10];
    bool dirty;                  // 0x238
};
static_assert(__builtin_offsetof(UnkText_b0080, dirty) == 0x238, "layout");

// The text's size: `size` in the frame's units (fn_71011b7e7c).
static inline void SetSize(UnkText_b0080* self, UnkGuiRoot* root, s32 frame, const CVector2D* size) {
    UnkLabelVec2 scaled = fn_71011b7e7c(root, frame, size);
    self->width = scaled.x;
    self->height = scaled.y;
}

void fn_7100b86de8(UnkText_b0080* self, void* font, const UnkTextValue_b0080* value) {
    UnkGuiRoot** root = &g_unk_717271e8f8;
    if (UnkLabelTextObject* old = self->object) {
        fn_71011baba0(*root, old, (old->bits >> 12) & 0xf);
        self->object = nullptr;
    }
    s32 frame = fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self))->frame;
    if (frame == 0x7fffffff)
        return;
    fn_71011bab10(*root, &self->object, frame);
    UnkLabelTextObject* object = self->object;
    object->font = font;
    fn_71001c3ec0(object->node, static_cast<char*>(font) + 0x18);
    object->field_0x11b = true;
    object = self->object;
    const s32* v = value->value;
    fn_71001c4418(object->node, v);
    object->field_0x11b = true;
    UnkGuiRoot* r = *root;
    f32 w = (f32)fn_71001c4510(self->object->node);
    f32 h = (f32)(u32)fn_71001bd59c(self->object->node->field_0x80);
    CVector2D size = {w, h};
    SetSize(self, r, frame, &size);
}

void fn_7100b86b84(UnkText_b0080* self);

void fn_7100b86f00(UnkText_b0080* self) {
    UnkGuiRoot** root = &g_unk_717271e8f8;
    UnkGuiDisplayObject* display = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011cee10(display);
    if (!self->dirty) {
        fn_7100b86b84(self);
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
    fn_7100b86de8(self, font, &self->value);
    fn_7100b86b84(self);
    self->dirty = false;
}

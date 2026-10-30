// The GUI's frame scale helpers (game/UnkGuiFrames.h), in the GUI manager's unit.
#include "game/UnkGuiFrames.h"
#include "base/math/CVector.h"
#include "game/UnkGuiRoot.h"

struct UnkGuiFrame {
    u8 pad_0x0[0x8];
    s32 key;
    u8 pad_0xc[0x18];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    u8 pad_0x3c[0xc];
};
static_assert(sizeof(UnkGuiFrame) == 0x48, "UnkGuiFrame layout");

// The GUI root's frames (game/UnkGuiRoot.h; the array at +0x40).
struct UnkGuiFrames {
    u8 pad_0x0[0x40];
    UnkGuiFrame* frames;
};

static inline UnkGuiFrame* FindFrame(s32 key) {
    UnkGuiFrame* frame = reinterpret_cast<UnkGuiFrames*>(g_unk_717271e8f8)->frames;
    while (frame->key != key)
        ++frame;
    return frame;
}

UnkLabelVec2 fn_71011b7f48(s32 key, const UnkLabelVec2& v) {
    UnkGuiFrame* f = FindFrame(key);
    return UnkLabelVec2(v.x, v.y * f->field_0x38);
}

UnkLabelVec2 fn_71011b7f88(s32 key, const UnkLabelVec2& v) {
    UnkGuiFrame* f = FindFrame(key);
    return UnkLabelVec2(v.x, v.y * f->field_0x34);
}

UnkLabelVec2 fn_71011b8078(s32 key, const UnkLabelVec2& v) {
    UnkGuiFrame* f = FindFrame(key);
    return UnkLabelVec2(v.x * f->field_0x28, v.y * f->field_0x30);
}

UnkLabelVec2 fn_71011b810c(s32 key, const UnkLabelVec2& v) {
    UnkGuiFrame* f = FindFrame(key);
    return UnkLabelVec2(f->field_0x28 * (v.x * f->field_0x24 + 0.5f), f->field_0x30 * (v.y * f->field_0x2c + 0.5f));
}

UnkLabelVec2 fn_71011b8160(s32 key, const UnkLabelVec2& v) {
    UnkGuiFrame* f = FindFrame(key);
    return UnkLabelVec2(f->field_0x28 * (v.x * f->field_0x24 + 0.5f), f->field_0x30 * (v.y * f->field_0x2c + 0.5f));
}

// The same over a given root: its frames (count at +0x50) and two root-wide scales.
struct UnkGuiFrameRoot {
    u8 pad_0x0[0x40];
    UnkGuiFrame* frames;
    u8 pad_0x48[8];
    u32 count;  // 0x50
    u8 pad_0x54[0xd44];
    f32 scale_0xd98[2];
    f32 scale_0xda0[2];
};
static_assert(__builtin_offsetof(UnkGuiFrameRoot, scale_0xda0) == 0xda0, "UnkGuiFrameRoot layout");

static inline UnkGuiFrameRoot* Frames(UnkGuiRoot* root) { return reinterpret_cast<UnkGuiFrameRoot*>(root); }

// The frame `key` names, or null.
UnkGuiFrame* fn_71011b7a4c(UnkGuiRoot* root, s32 key) {
    UnkGuiFrameRoot* r = Frames(root);
    for (UnkGuiFrame *f = r->frames, *end = f + r->count; f != end; ++f)
        if (f->key == key)
            return f;
    return nullptr;
}

static inline UnkGuiFrame* FindFrameIn(UnkGuiFrameRoot* r, s32 key) {
    UnkGuiFrame* frame = r->frames;
    while (frame->key != key)
        ++frame;
    return frame;
}

// NON_MATCHING(minor): register allocation only (the products go to other registers).
UnkLabelVec2 fn_71011b7e7c(UnkGuiRoot* root, s32 key, const base::math::CVector2D* v) {
    UnkGuiFrameRoot* r = Frames(root);
    UnkGuiFrame* f = FindFrameIn(r, key);
    return UnkLabelVec2(v->x * r->scale_0xd98[0] * f->field_0x28, v->y * r->scale_0xd98[1] * f->field_0x30);
}

// NON_MATCHING(minor): as fn_71011b7e7c.
UnkLabelVec2 fn_71011b7fc8(UnkGuiRoot* root, s32 key, const base::math::CVector2D* v) {
    UnkGuiFrameRoot* r = Frames(root);
    UnkGuiFrame* f = FindFrameIn(r, key);
    return UnkLabelVec2(v->x * r->scale_0xda0[0] * f->field_0x28, v->y * r->scale_0xda0[1] * f->field_0x30);
}

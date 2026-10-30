// The GUI's frame scale helpers (game/UnkGuiFrames.h), in the GUI manager's unit.
#include "game/UnkGuiFrames.h"
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

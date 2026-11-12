// GUI::CLabel unit (tools/tu_map.py, the name from reflection). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU135.h"
#include "game/UnkTU2744.h"
#include "types.h"

using base::global::CStrId;

struct UnkLabelFontView_b52 {
    u8 pad_0x0[0x260];
    base::UnkStrIdMember font_path;  // 0x260
    u8 pad_0x268[0x30];
    void* font;                      // 0x298
    u8 pad_0x2a0[0x10];
    s32 line_height;                 // 0x2b0: negative to take the font's (its height of ' ')
    u8 pad_0x2b4[0x50];
    s32 lines;                       // 0x304: negative to compute them
};

static inline UnkGuiObjectState* State(UnkLabelFontView_b52* self) {
    return fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

// The font the label's path names, looked up once in the GUI root's cache (the lookup takes the path
// as a second argument). Null while the label has no frame or no path.
static inline void* Font(UnkLabelFontView_b52* self) {
    if (!self->font && State(self)->frame != 0x7fffffff && self->font_path.data != &g_unk_7101d47108->empty) {
        auto lookup = reinterpret_cast<void* (*)(UnkGuiRoot*, const base::UnkStrIdMember*)>(fn_71011b882c);
        self->font = lookup(g_unk_717271e8f8, &self->font_path);
    }
    return self->font;
}

// How many lines fit the label: `lines` when set, else its frame's height over the line height,
// rounded down; at least 1.
s32 fn_71011eedd0(UnkLabelFontView_b52* self) {
    if (self->lines >= 0)
        return self->lines;
    UnkGuiObjectState* state = State(self);
    if (state->frame == 0x7fffffff)
        return 1;
    void* font = Font(self);
    if (!font)
        return 1;
    s32 height = self->line_height;
    if (height < 0)
        height = fn_71001479dc(font, 0x20);
    if (height == 0)
        return 1;
    UnkLabelVec2 size = fn_71011b800c(g_unk_717271e8f8, state->frame,
                                      reinterpret_cast<const UnkLabelVec2*>(&state->unk_0x0[0x14]));
    u32 n = (u32)floorf(size.y / (f32)(u32)height);
    return n ? n : 1;
}

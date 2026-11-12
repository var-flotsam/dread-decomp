// GUI::CLabel unit (tools/tu_map.py, the name from reflection). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2744.h"
#include "types.h"

using base::global::CStrId;
struct UnkLabelLayout;

struct UnkLabelFontView_b53 {
    u8 pad_0x0[0x260];
    base::UnkStrIdMember font_path;  // 0x260
    u8 pad_0x268[0x30];
    void* font;                      // 0x298
};

static inline UnkGuiObjectState* State(UnkLabelFontView_b53* self) {
    return fn_71011cde14(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

static inline void* Font(UnkLabelFontView_b53* self) {
    if (!self->font && State(self)->frame != 0x7fffffff && self->font_path.data != &g_unk_7101d47108->empty) {
        auto lookup = reinterpret_cast<void* (*)(UnkGuiRoot*, const base::UnkStrIdMember*)>(fn_71011b882c);
        self->font = lookup(g_unk_717271e8f8, &self->font_path);
    }
    return self->font;
}

// Re-lays the label's text out once its font is known, then refreshes it.
void fn_71011ef0c8(UnkLabelFontView_b53* self, bool only_if_shown) {
    if (!Font(self))
        return;
    fn_71011ed1e8(reinterpret_cast<UnkLabelLayout*>(self), only_if_shown);
    fn_71011ec768(reinterpret_cast<UnkLabelLayout*>(self));
}

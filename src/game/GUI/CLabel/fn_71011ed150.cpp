// In GUI::CLabel's unit (unit 2752 of tools/tu_map.py): sets the label's font (a string id at
// +0x260); on a change it clears +0x298 and has the label lay itself out again (vf_0x88). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"

using base::global::CStrId;

struct UnkGuiLabelFont {
    u8 pad_0x0[0x260];
    CStrId font;
    u8 pad_0x268[0x30];
    u64 field_0x298;
};

void fn_71011ed150(UnkGuiDisplayObject* label, const CStrId& font) {
    UnkGuiLabelFont* self = reinterpret_cast<UnkGuiLabelFont*>(label);
    if (font.data != self->font.data) {
        self->font = font;
        self->field_0x298 = 0;
        label->vf_0x88();
    }
}

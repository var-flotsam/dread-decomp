// GUI::CIntTunableButton's setup (after its base step fn_71011bfd28): creates the "TunableLabel" label
// as GUI::CDebugTunableButton's does, keeps it at +0x2a8 and shows the value (+0x2b0) in it: the
// option's text from the list at +0x2b8 when there is one, the number otherwise; then tells the script
// named at +0x2e0, if any, the value. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkStruct_71011ebdb4;
struct UnkMissionLogButton_ovb0079;
struct UnkRumbleTunableButton_ovb0085;

struct UnkTunableLabelView_lb32578 {
    u8 pad_0x0[0x130];
    CStrId name;
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[2];
    bool field_0x187;
    bool field_0x188;
};
static_assert(__builtin_offsetof(UnkTunableLabelView_lb32578, field_0x188) == 0x188, "layout");

// One option's text, as the label's text setter takes it.
struct UnkIntTunableOptionText_lb32578 {
    u8 pad_0x0[0x10];
};

struct UnkIntTunableOptions_lb32578 {
    u8 pad_0x0[0xf0];
    UnkIntTunableOptionText_lb32578* texts;  // 0xf0
};

struct UnkIntTunableButton_lb32578 {
    u8 pad_0x0[0x2a8];
    UnkGuiDisplayObject* label;              // 0x2a8
    u32 value;                               // 0x2b0
    u8 pad_0x2b4[0x4];
    UnkIntTunableOptions_lb32578* options;   // 0x2b8
    u8 pad_0x2c0[0x2e0 - 0x2c0];
    CStrId script;                           // 0x2e0
};
static_assert(__builtin_offsetof(UnkIntTunableButton_lb32578, script) == 0x2e0, "layout");

static inline UnkGuiDisplayObject* Sub(UnkIntTunableButton_lb32578* self) {
    return fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self));
}

void fn_7100b32578(UnkIntTunableButton_lb32578* self) {
    fn_71011bfd28(reinterpret_cast<UnkRumbleTunableButton_ovb0085*>(self));
    UnkGuiRoot* root = g_unk_717271e8f8;
    {
        CStrId id("TunableLabel", true);
        auto* label = static_cast<UnkGuiDisplayObject*>(fn_71000839a0(0x340, 0xc));
        fn_71011ebdb4(reinterpret_cast<UnkStruct_71011ebdb4*>(label));
        if (label) {
            label->vf_0x18();
            auto* v = reinterpret_cast<UnkTunableLabelView_lb32578*>(label);
            v->name = id;
            if (!v->field_0x187 &&
                (v->field_0x188 || v->field_0x184 ||
                 (fn_71011cde14(label)->field_0x3c && (fn_71011ceac0(label) || fn_71011ceb34(label)))))
                fn_71011b5598(root, label);
        }
        self->label = label;
    }
    fn_71011cdce8(self->label, true);
    {
        UnkGuiDisplayObject* label = self->label;
        CStrId font("digital_medium", true);
        fn_71011ed150(label, font);
    }
    fn_71011d0088(self->label, false);
    fn_71011ed018(self->label, 2);
    fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(self), self->label);
    if (self->options) {
        u32 index = self->value;
        const UnkIntTunableOptionText_lb32578* text = &self->options->texts[index];
        fn_71011ecfbc(self->label, reinterpret_cast<const char32_t*>(text));
    } else {
        CRntString text(g_unk_710162e2ed);
        fn_710007f634(&text, self->value);
        fn_71011ec424(self->label, text);
    }
    fn_71011cd5a0(self->label);
    fn_71011ed018(Sub(self), 0);
    if (self->script.data != &g_unk_7101d47108->empty)
        fn_7100000790(self->script.Text(), "i", self->value);
}

// GUI::CFloatTunableButton's unit (tools/tu_map.py, unit 1676): its setup after the base step (fn_71011bfd28).
// Creates the "TunableLabel" label (+0x2a8) in the digital_medium font, adds it to the button, shows the value
// on it and, when the button has a script name, calls that script function with the value. Names and layouts
// are placeholders.
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

struct UnkFloatTunableLabelView_lb1f960 {
    u8 pad_0x0[0x130];
    CStrId name;
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[2];
    bool field_0x187;
    bool field_0x188;
};
static_assert(__builtin_offsetof(UnkFloatTunableLabelView_lb1f960, field_0x188) == 0x188, "layout");

// The script name: a pooled string whose use count is at +0x44 and text at +0x10.
struct UnkFloatScriptName_lb1f960 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    s32 use_count;  // 0x44
};

struct UnkFloatTunableButton_lb1f960 {
    u8 pad_0x0[0x2a8];
    UnkGuiDisplayObject* label;  // 0x2a8
    f32 value;                   // 0x2b0
    u8 pad_0x2b4[0x2d0 - 0x2b4];
    UnkFloatScriptName_lb1f960* script;  // 0x2d0
};

static inline UnkGuiDisplayObject* Sub(UnkFloatTunableButton_lb1f960* self) {
    return fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self));
}

void fn_7100b1f960(UnkFloatTunableButton_lb1f960* self) {
    fn_71011bfd28(reinterpret_cast<UnkRumbleTunableButton_ovb0085*>(self));
    UnkGuiRoot* root = g_unk_717271e8f8;
    {
        CStrId id("TunableLabel", true);
        auto* label = static_cast<UnkGuiDisplayObject*>(fn_71000839a0(0x340, 0xc));
        fn_71011ebdb4(reinterpret_cast<UnkStruct_71011ebdb4*>(label));
        if (label) {
            label->vf_0x18();
            auto* v = reinterpret_cast<UnkFloatTunableLabelView_lb1f960*>(label);
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
    CRntString text(g_unk_710162e2ed);
    CRntString_AppendFloat(&text, self->value);
    fn_71011ec424(self->label, text);
    fn_71011cd5a0(self->label);
    fn_71011ed018(Sub(self), 0);
    UnkFloatScriptName_lb1f960* script = self->script;
    if (script != reinterpret_cast<UnkFloatScriptName_lb1f960*>(&g_unk_7101d47108->empty)) {
        ++script->use_count;
        const char* name = self->script->text;
        fn_7100000790(name ? name : g_unk_710162e2ed, "f", (f64)self->value);
    }
}

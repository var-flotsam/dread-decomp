// GUI::CDebugTunableButton's setup (after its base step fn_71011bfd28): creates the "TunableLabel" label
// (as src/game/UnkLoadingScreen.cpp's NewLabel does), keeps it at +0x2a8, shows it in the digital_medium
// font and adds it to the button. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71011ebdb4;
bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void fn_71011ed150(UnkGuiDisplayObject* label, const CStrId& font);
struct UnkGuiObj;
struct UnkMissionLogButton_ovb0079;

struct UnkTunableLabelView_b0266 {
    u8 pad_0x0[0x130];
    CStrId name;
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[2];
    bool field_0x187;
    bool field_0x188;
};
static_assert(__builtin_offsetof(UnkTunableLabelView_b0266, field_0x188) == 0x188, "layout");

struct UnkDebugTunableButton_b0266 {
    u8 pad_0x0[0x2a8];
    UnkGuiDisplayObject* label;  // 0x2a8
};

struct UnkRumbleTunableButton_ovb0085;

static inline UnkGuiDisplayObject* Sub(UnkDebugTunableButton_b0266* self) {
    return reinterpret_cast<UnkGuiDisplayObject*>(
        fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self)));
}

void fn_7100b00398(UnkDebugTunableButton_b0266* self) {
    fn_71011bfd28(reinterpret_cast<UnkRumbleTunableButton_ovb0085*>(self));
    UnkGuiRoot* root = g_unk_717271e8f8;
    {
        CStrId id("TunableLabel", true);
        auto* label = static_cast<UnkGuiDisplayObject*>(fn_71000839a0(0x340, 0xc));
        fn_71011ebdb4(reinterpret_cast<UnkStruct_71011ebdb4*>(label));
        if (label) {
            label->vf_0x18();
            auto* v = reinterpret_cast<UnkTunableLabelView_b0266*>(label);
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
    fn_71011ed084(Sub(self), 1);
    fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(self), self->label);
    fn_71011ed018(Sub(self), 0);
    fn_71011ed084(Sub(self), 1);
}

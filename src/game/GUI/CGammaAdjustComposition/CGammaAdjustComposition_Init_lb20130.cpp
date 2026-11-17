// GUI::CGammaAdjustComposition's unit (tools/tu_map.py unit 1678): the composition's set-up: runs the base
// set-up, finds the "Content" container once, and in it the gamma slider's bar and handle sprites; then sets
// the slider's step count. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

struct UnkGammaAdjustComposition_lb20130 {
    u8 pad_0x0[0x23c];
    s32 field_0x23c;
    UnkGuiDisplayObject* content;        // 0x240
    UnkGuiDisplayObject* slider_bar;     // 0x248
    UnkGuiDisplayObject* slider_handle;  // 0x250
};
static_assert(__builtin_offsetof(UnkGammaAdjustComposition_lb20130, content) == 0x240, "layout");

// The object when it is of the class `Class` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

void fn_7100b20130(UnkGammaAdjustComposition_lb20130* self) {
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    if (!self->content)
        self->content = As<fn_71011d7b84>(
            fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(self), CStrId("Content", true)));
    UnkGuiDisplayObject* slider =
        As<fn_71011d7b84>(fn_71011d94d4(self->content, CStrId("SliderComposition-Gamma", true)));
    if (slider) {
        self->slider_bar = As<fn_71011f6890>(fn_71011d94d4(slider, CStrId("SliderBar", true)));
        self->slider_handle = As<fn_71011f6890>(fn_71011d94d4(slider, CStrId("SliderHandle", true)));
    }
    self->field_0x23c = 20;
}

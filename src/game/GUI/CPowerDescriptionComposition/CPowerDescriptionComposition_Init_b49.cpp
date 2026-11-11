// GUI::CPowerDescriptionComposition's unit (tools/tu_map.py): its set-up after the base one
// (fn_71011d9b3c): looks its parts up by path (fn_71011d9508): the description label (a GUI label,
// fn_71011e8dfc; +0x2d8, its word at +0x2a4 set to 1), the scroll bar composition (+0x2c0) and in it
// (when it is a GUI::CDisplayObjectContainer) the "Handle" (+0x2d0) and "Bar" (+0x2c8), and the
// continue-down and continue-up labels (+0x2e8, +0x2f0), which get the big arrow texts
// (fn_71011ecfbc). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

using UnkGuiObj = UnkGuiDisplayObject;
struct UnkPanel_71011dfeb0;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
void fn_71011ecfbc(UnkGuiObj* label, const char32_t* text);

// Looks a child up by path (declared as src/game_s7/UnkGuiS7.h does: the path string by address).
static inline UnkGuiObj* FindByPath(void* self, const CRntString& path) {
    return fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
}

// Sets a label's text from a string id (declared as src/game_s7/UnkGuiS7.h does: the text by address).
static inline void SetText(UnkGuiObj* label, const CStrId& text) {
    fn_71011ecfbc(label, reinterpret_cast<const char32_t*>(&text));
}

struct UnkGuiLabelWord {
    u8 pad_0x0[0x2a4];
    u32 field_0x2a4;
};

struct UnkPowerDescCompositionInit {
    u8 pad_0x0[0x2c0];
    UnkGuiDisplayObject* scroll_bar;      // 0x2c0
    UnkGuiDisplayObject* bar;             // 0x2c8
    UnkGuiDisplayObject* handle;          // 0x2d0
    UnkGuiDisplayObject* description;     // 0x2d8
    u8 pad_0x2e0[0x8];
    UnkGuiDisplayObject* continue_down;   // 0x2e8
    UnkGuiDisplayObject* continue_up;     // 0x2f0
};
static_assert(__builtin_offsetof(UnkPowerDescCompositionInit, continue_up) == 0x2f0, "layout");

template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

void fn_7100b63848(UnkPowerDescCompositionInit* self) {
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    self->description = As<fn_71011e8dfc>(FindByPath(self, CRntString("PopUpComposition.Content.DescriptionLabel")));
    reinterpret_cast<UnkGuiLabelWord*>(self->description)->field_0x2a4 = 1;
    self->scroll_bar = FindByPath(self, CRntString("PopUpComposition.Content.ScrollBarComposition"));
    self->handle = fn_71011d94d4(As<fn_71011d7b84>(self->scroll_bar), CStrId("Handle", true));
    self->bar = fn_71011d94d4(As<fn_71011d7b84>(self->scroll_bar), CStrId("Bar", true));
    self->continue_down = As<fn_71011e8dfc>(FindByPath(self, CRntString("PopUpComposition.Content.ContinueDown")));
    if (self->continue_down)
        SetText(self->continue_down, CStrId("#GUI_ARROW_DOWN_BIG", true));
    self->continue_up = As<fn_71011e8dfc>(FindByPath(self, CRntString("PopUpComposition.Content.ContinueUp")));
    if (self->continue_up)
        SetText(self->continue_up, CStrId("#GUI_ARROW_UP_BIG", true));
}

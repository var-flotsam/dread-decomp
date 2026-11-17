// GUI::CGammaAdjustComposition's unit (tools/tu_map.py unit 1678): the composition's event handler: after
// the base handler, on event kind 2 it shows "Back" and one of the two instruction texts by fn_7100deac24,
// then places the slider's handle at the step for the current gamma and plays a sound. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CStrId;

struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};

// As src/game/GUI/CDisplayObject.cpp defines it.
void fn_71011cc65c(f32 x, UnkGuiDisplayObject* self);

extern UnkStateKey g_unk_71729f0298;  // The gamma slider's sound event.

struct UnkGraphicSettings_lb203ec {
    u8 pad_0x0[0x24];
    f32 gamma;  // 0x24
};

struct UnkGuiPlacement_lb203ec {
    u8 pad_0x0[0x8c];
    f32 field_0x8c;
};

struct UnkGammaAdjustComposition_lb203ec {
    u8 pad_0x0[0x230];
    f32 field_0x230;
    f32 field_0x234;
    s32 step;                            // 0x238
    s32 step_count;                      // 0x23c
    UnkGuiDisplayObject* content;        // 0x240
    UnkGuiDisplayObject* slider_bar;     // 0x248
    UnkGuiDisplayObject* slider_handle;  // 0x250
};
static_assert(__builtin_offsetof(UnkGammaAdjustComposition_lb203ec, content) == 0x240, "layout");

// The object when it is of the class `Class` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

static inline f32 PlacementValue(UnkGuiDisplayObject* obj) {
    return reinterpret_cast<UnkGuiPlacement_lb203ec*>(obj)->field_0x8c;
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    f32 r = v >= hi ? hi : v;
    if (v <= lo)
        return lo;
    return r;
}

void fn_7100b203ec(UnkGammaAdjustComposition_lb203ec* self, UnkGuiEvent_b49* event) {
    fn_71011d99e4(reinterpret_cast<UnkGuiDisplayObject*>(self), event);
    if (event->kind != 2)
        return;

    UnkGuiDisplayObject* back = As<fn_71011c3868>(fn_71011d94d4(self->content, CStrId("Back", true)));
    if (back)
        fn_71011cdce8(back, !fn_7100deac24(g_unk_7101d47310));

    if (fn_7100deac24(g_unk_7101d47310)) {
        UnkGuiDisplayObject* text = As<fn_71011c3868>(fn_71011d94d4(self->content, CStrId("Instructions", true)));
        UnkGuiDisplayObject* other =
            As<fn_71011c3868>(fn_71011d94d4(self->content, CStrId("Instructions-1", true)));
        fn_71011cdce8(text, false);
        fn_71011cdce8(other, true);
    } else {
        UnkGuiDisplayObject* text = As<fn_71011c3868>(fn_71011d94d4(self->content, CStrId("Instructions", true)));
        UnkGuiDisplayObject* other =
            As<fn_71011c3868>(fn_71011d94d4(self->content, CStrId("Instructions-1", true)));
        fn_71011cdce8(text, true);
        fn_71011cdce8(other, false);
    }

    const f32 kMinGamma = 1.2f;
    const f32 kMaxGamma = 3.2f;
    f32 gamma = Clamp(reinterpret_cast<UnkGraphicSettings_lb203ec*>(g_unk_71727be8a8)->gamma, kMinGamma, kMaxGamma);
    self->field_0x230 = self->field_0x234 = gamma;
    self->step = (s32)((gamma - kMinGamma) / (kMaxGamma - kMinGamma) * self->step_count + 0.5f);
    f32 width = PlacementValue(self->slider_bar) - PlacementValue(self->slider_handle);
    fn_71011cc65c(width / self->step_count * self->step, self->slider_handle);
    fn_71011cd5a0(self->slider_handle);
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0298, self);
}

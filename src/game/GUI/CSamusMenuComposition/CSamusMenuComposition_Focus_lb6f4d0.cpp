// GUI::CSamusMenuComposition's unit (1714): taking focus. The composition remembers the name of the focused
// object; when it is the bombs list, Samus's portrait turns to her morph ball (and back otherwise), with the
// matching animation, and the "Ball" or "Base" sprite is looked up. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CStrId;

// A display object as this file uses it: its animation call.
struct UnkPortraitWidget_lb6f4d0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void PlayAnimation(const char* name, bool loop);  // vf_0x1e8
};

// The focused object: its name.
struct UnkFocusedObject_lb6f4d0 {
    u8 pad_0x0[0x130];
    CStrId name;  // 0x130
};

// Which form the portrait shows.
enum class UnkSamusForm_lb6f4d0 : u8 {
    Power = 0,
    Ball = 1,
};

struct UnkSamusMenuComposition_lb6f4d0 {
    u8 pad_0x0[0x2d0];
    CStrId focused_name;              // 0x2d0
    UnkGuiDisplayObject* form_sprite;  // 0x2d8
    u8 pad_0x2e0[0x18];
    UnkSamusForm_lb6f4d0 form;  // 0x2f8
    bool focused_before;         // 0x2f9
};
static_assert(__builtin_offsetof(UnkSamusMenuComposition_lb6f4d0, focused_before) == 0x2f9, "");

struct UnkGuiButtonFocus_ovb0000;

template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

void fn_7100b6f4d0(UnkSamusMenuComposition_lb6f4d0* self) {
    fn_71011d9ab4(reinterpret_cast<UnkGuiButtonFocus_ovb0000*>(self));
    self->focused_name = base::global::CStrId::Empty();
    self->form_sprite = nullptr;
    UnkWeakHandle* focus = g_unk_717271e8f8->field_0x230;
    if (!focus || !focus->target)
        return;
    auto* focused = reinterpret_cast<UnkFocusedObject_lb6f4d0*>(focus->target);
    if (self->focused_before)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0188, self);
    self->focused_before = true;
    self->focused_name = focused->name;

    UnkGuiDisplayObject* portrait = As<fn_71011d7b84>(
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(self), CStrId("PORTRAITCAM_SAMUS", true)));
    const UnkSamusForm_lb6f4d0 was = self->form;
    if (self->focused_name == CStrId("BombsListComposition", true)) {
        self->form = UnkSamusForm_lb6f4d0::Ball;
        if (was != UnkSamusForm_lb6f4d0::Ball)
            reinterpret_cast<UnkPortraitWidget_lb6f4d0*>(portrait)->PlayAnimation("SamusPortrait_PowerToBall",
                                                                                    false);
    } else if (self->form != UnkSamusForm_lb6f4d0::Power) {
        self->form = UnkSamusForm_lb6f4d0::Power;
        if (was != UnkSamusForm_lb6f4d0::Power)
            reinterpret_cast<UnkPortraitWidget_lb6f4d0*>(portrait)->PlayAnimation("SamusPortrait_BallToPower",
                                                                                    false);
    }
    self->form_sprite = nullptr;
    if (self->form != UnkSamusForm_lb6f4d0::Power)
        self->form_sprite = As<fn_71011f6890>(fn_71011d94d4(portrait, CStrId("Ball", true)));
    else
        self->form_sprite = As<fn_71011f6890>(fn_71011d94d4(portrait, CStrId("Base", true)));
}

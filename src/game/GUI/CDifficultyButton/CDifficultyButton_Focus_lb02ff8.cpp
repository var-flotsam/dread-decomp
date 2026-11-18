// GUI::CDifficultyButton's unit (tools/tu_map.py unit 1666): gaining focus. After the button's own focus
// step it fills the main menu's pop-up: the description label gets the button's text (+0x2b0), the image
// sprite the button's image (+0x2e0), greyed when the difficulty is locked (+0x2a0), the "Accept" label is
// dimmed when locked, and the lock icon shows the lock. Names and layouts are placeholders.
#include "../../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiSprite.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkGuiButtonFocus_ovb0000;
void fn_71011bf558(UnkGuiButtonFocus_ovb0000* self);

struct UnkColor_lb02ff8 {
    f32 r, g, b, a;
};

// Fills `color` with white (four ones).
void fn_71011c1728(void* color);

// A display object's colour (+0xa0), as the label refresh below passes it back.
struct UnkGuiColored_lb02ff8 {
    u8 pad_0x0[0xa0];
    UnkColor_lb02ff8 color;  // 0xa0
};
void fn_71011ce488(UnkGuiDisplayObject* self, const UnkColor_lb02ff8* color);

struct UnkMainMenu_lb02ff8 {
    u8 pad_0x0[0x4f0];
    UnkGuiDisplayObject* root;  // 0x4f0
};

struct UnkDifficultyButton_lb02ff8 {
    u8 pad_0x0[0x2a0];
    bool locked;  // 0x2a0
    u8 pad_0x2a1[0xf];
    CRntString description;  // 0x2b0
    CStrId image;            // 0x2e0
};
static_assert(__builtin_offsetof(UnkDifficultyButton_lb02ff8, image) == 0x2e0, "layout");

// Looks a child up by path (declared as src/game_s7/UnkGuiS7.h does: the path string by address).
static inline UnkGuiDisplayObject* FindByPath(UnkGuiDisplayObject* self, const CRntString& path) {
    return fn_71011d9508(self, reinterpret_cast<const char* const*>(&path));
}

// The object when it is of the class `Class` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Class)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(Class()))
               ? obj
               : nullptr;
}

// Greys the sprite out when locked, else makes it white.
static inline void Tint(UnkGuiDisplayObject* sprite, bool locked) {
    UnkColor_lb02ff8 color;
    if (locked)
        color = {0.7f, 0.7f, 0.7f, 0.7f};
    else
        fn_71011c1728(&color);
    unk_gui::fn_71011cfc24(sprite, reinterpret_cast<const unk_gui::UnkColor&>(color), true);
}

void fn_7100b02ff8(UnkDifficultyButton_lb02ff8* self) {
    fn_71011bf558(reinterpret_cast<UnkGuiButtonFocus_ovb0000*>(self));
    UnkReflObject* object = fn_7100e033a8(g_unk_7101d47310);
    auto* menu = reinterpret_cast<UnkMainMenu_lb02ff8*>(
        fn_710009d50c(object->vf_0x0(), fn_7100b34230()) ? object : nullptr);
    UnkGuiDisplayObject* root = menu->root;
    if (!root)
        return;
    UnkGuiDisplayObject* popup = As<fn_71011d7b84>(FindByPath(root, CRntString("Content.PopUpComposition")));
    if (!popup)
        return;

    UnkGuiDisplayObject* label =
        As<fn_71011e8dfc>(FindByPath(popup, CRntString("DescriptionComposition.DescriptionLabel")));
    fn_71011ec424(label, self->description);
    label->vf_0x88();
    fn_71011cd5a0(label);

    UnkGuiDisplayObject* sprite = As<fn_71011f6890>(fn_71011d94d4(popup, CStrId("ImgSprite", true)));
    fn_71011f8bdc(sprite, self->image);
    Tint(sprite, self->locked);

    UnkGuiDisplayObject* accept = As<fn_71011e8dfc>(fn_71011d98ec(root, CStrId("Accept", true).data->Key()));
    fn_71011ce488(accept, &reinterpret_cast<UnkGuiColored_lb02ff8*>(accept)->color);
    fn_71011cec68(accept, self->locked ? 0.35f : 1.0f);
    fn_71011ce208(accept, 0.2f);

    UnkGuiDisplayObject* icon = As<fn_71011f6890>(fn_71011d98ec(root, fn_7100001570("LockedIcon", 10)));
    fn_71011cdc30(icon, self->locked);
}

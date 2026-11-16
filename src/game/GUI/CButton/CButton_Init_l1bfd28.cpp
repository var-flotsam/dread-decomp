// GUI::CButton's unit (tools/tu_map.py unit 2745): the button's set-up after the base composition's: it keeps a
// weak handle to its owner, finds its sprite (shown unfocused) and its label among its children by the game's
// string ids, and sends itself event 0x2b. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CSprite.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "../../../game_s7/UnkGuiS7.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

UnkWeakTarget* fn_71011cef5c(UnkGuiDisplayObject* self);

// The id table (its slots at +0x8a8 and +0x1588 are the sprite and label names).
inline const UnkGameManagerIdTable& ButtonIds_l1bfd28() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

struct UnkButtonColor_l1bfd28 {
    f32 r, g, b, a;
    UnkButtonColor_l1bfd28(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkButtonSprite_l1bfd28 {
    u8 pad_0x0[0xac];
    f32 alpha;  // 0xac
};

struct UnkGuiButton_l1bfd28 : UnkGuiDisplayObject {
    u8 pad_0x198[0x250 - 0x198];
    bool tint_on_focus;  // 0x250
    bool fade_on_focus;  // 0x251
    u8 pad_0x252[0x270 - 0x252];
    UnkWeakHandle* owner;           // 0x270
    UnkGuiDisplayObject* sprite;    // 0x278
    UnkGuiDisplayObject* label;     // 0x280
    u8 pad_0x288[0x28c - 0x288];
    f32 alpha;  // 0x28c

    void SendEvent(UnkGuiEvent* event) {
        (*reinterpret_cast<void (***)(UnkGuiButton_l1bfd28*, UnkGuiEvent*)>(this))[0xd8 / 8](this, event);
    }
    void Notify(const char* event, bool flag) {
        (*reinterpret_cast<void (***)(UnkGuiButton_l1bfd28*, const char*, bool)>(this))[0x1e8 / 8](this, event,
                                                                                                    flag);
    }
    void ShowUnfocused() {
        if (tint_on_focus) {
            UnkGuiDisplayObject* s = sprite;
            alpha = reinterpret_cast<UnkButtonSprite_l1bfd28*>(s)->alpha;
            UnkButtonColor_l1bfd28 color(0.4f, 0.4f, 0.4f, alpha);
            unk_gui::fn_71011cfc24(s, reinterpret_cast<const unk_gui::UnkColor&>(color), true);
            fn_71011f9878(sprite);
        } else if (fade_on_focus) {
            unk_gui::fn_71011cfba8(sprite, 0.0f);
            fn_71011f9878(sprite);
        } else {
            Notify("OnFocusLost", false);
        }
    }
};

void fn_71011bfd28(UnkRumbleTunableButton_ovb0085* button) {
    UnkGuiButton_l1bfd28* self = reinterpret_cast<UnkGuiButton_l1bfd28*>(button);
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    if (!self->owner || !self->owner->target) {
        UnkWeakHandle* handle = GetWeakHandle(fn_71011cef5c(self));
        AssignRef(&self->owner, handle);
    }
    if (!self->sprite) {
        UnkGuiDisplayObject* child = fn_71011d94d4(self, ButtonIds_l1bfd28().field_0x8a8);
        self->sprite = child && fn_710009d50c(child->GetClass(), fn_71011f6890()) ? child : nullptr;
        fn_71011d0088(self->sprite, false);
        self->ShowUnfocused();
    }
    if (!self->label) {
        UnkGuiDisplayObject* child = fn_71011d94d4(self, ButtonIds_l1bfd28().field_0x1588);
        if (child && fn_710009d50c(child->GetClass(), fn_71011e8dfc())) {
            self->label = child;
            fn_71011d0088(child, false);
        } else {
            self->label = nullptr;
        }
    }
    UnkGuiEvent event;
    s32 kind = 0x2b;
    fn_710120235c(&event, self, &kind, nullptr);
    self->SendEvent(&event);
    fn_7101202488(&event);
}

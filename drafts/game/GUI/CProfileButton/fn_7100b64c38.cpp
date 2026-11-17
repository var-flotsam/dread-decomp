// GUI::CProfileButton's unit (tools/tu_map.py): with the main menu up, reads the summary of the button's
// profile slot (+0x2a8) and shows either the "EmptyContent" container or the "SlotInfoComposition" (told
// which slot it shows), then runs the base update. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

void fn_7100df9728(UnkGameManager* manager);

struct UnkProfileButton_lb64c38;
void fn_71011bf3a0(UnkProfileButton_lb64c38* self);

struct UnkProfileButton_lb64c38 {
    u8 pad_0x0[0x2a8];
    s32 slot;  // 0x2a8
};

struct UnkProfilePart_lb64c38 {
    u8 pad_0x0[0x88];
    u8 field_0x88[0x90];
};

struct UnkProfile_lb64c38 {
    u8 pad_0x0[0x28];
    UnkProfilePart_lb64c38 field_0x28;
};
static_assert(sizeof(UnkProfile_lb64c38) == 0x140, "");

struct UnkMainMenu_lb64c38 {
    u8 pad_0x0[0x50];
    UnkProfile_lb64c38 profiles[3];  // 0x50

    // The profile in the given slot; the first one when the slot is out of range.
    UnkProfile_lb64c38* GetProfile(s32 index) {
        return &profiles[index >= 0 && index < 3 ? index : 0];
    }
};

// What the slot info shows of a profile: whether it holds a game, and the rest.
struct UnkSlotSummary_lb64c38 {
    bool valid;
    u8 pad_0x1[0x7f];
    CRntString field_0x80;

    UnkSlotSummary_lb64c38(void* profile) asm("fn_7100afa4c0");
};

struct UnkSlotInfoComposition_lb64c38 {
    u8 pad_0x0[0x230];
    s32 slot;  // 0x230
};

// The object when it is of the class `Type` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

void fn_7100b64c38(UnkProfileButton_lb64c38* self) {
    auto* menu = static_cast<UnkMainMenu_lb64c38*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    if (menu) {
        fn_7100df9728(g_unk_7101d47310);
        UnkProfile_lb64c38* profile = menu->GetProfile(static_cast<s8>(self->slot));
        UnkGuiDisplayObject* button = reinterpret_cast<UnkGuiDisplayObject*>(self);
        UnkGuiDisplayObject* empty =
            As<fn_71011d7b84>(fn_71011d94d4(button, CStrId("EmptyContent", true)));
        UnkSlotSummary_lb64c38 summary(profile->field_0x28.field_0x88);
        if (summary.valid) {
            if (empty)
                fn_71011cdce8(empty, false);
            UnkGuiDisplayObject* info =
                As<fn_71011d7b84>(fn_71011d94d4(button, CStrId("SlotInfoComposition", true)));
            if (info) {
                if (UnkGuiDisplayObject* slot_info = As<fn_7100b79b74>(info))
                    reinterpret_cast<UnkSlotInfoComposition_lb64c38*>(slot_info)->slot = self->slot;
                fn_71011cdce8(info, true);
            }
        } else {
            if (empty)
                fn_71011cdce8(empty, true);
            UnkGuiDisplayObject* info =
                As<fn_71011d7b84>(fn_71011d94d4(button, CStrId("SlotInfoComposition", true)));
            if (info)
                fn_71011cdce8(info, false);
        }
    }
    fn_71011bf3a0(self);
}

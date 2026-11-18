// GUI::CHUD (unit 2751): on a power bomb change (the item, its ammo or its maximum), unless the HUD is
// hidden or a bomb animation is playing, shows "current / max" on the power bomb counter when the item is
// owned and fires the "OnBombFill" and "OnBombEmpty" events. Names and layouts are placeholders.
#include <stdio.h>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CInventoryComponent.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflObject;

struct UnkInventory;
using UnkGuiObj = UnkGuiDisplayObject;

// GUI::CHUD::CBombAnimation's class (what fn_71011dd7e0 returns).
__attribute__((visibility("hidden"))) extern base::reflection::CClass g_unk_7172a10410;

// A reference to a display object of the HUD's layout.
struct UnkHudRef_42cc {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
};

struct UnkHud_42cc;

struct UnkHudVtable_42cc {
    void* slots[0x3d];
    void (*SendEvent)(UnkHud_42cc* self, const char* name, bool flag);  // 0x1e8
};

struct UnkHud_42cc {
    UnkHudVtable_42cc* vtable;
    u8 pad_0x8[0x22e];
    bool hidden;  // 0x236
    u8 pad_0x237[0x39];
    UnkHudRef_42cc* bomb_label;  // 0x270
    u8 pad_0x278[0x20];
    UnkHudRef_42cc* bomb_icon;  // 0x298
    u8 pad_0x2a0[0xa0];
    UnkReflObject** animations;  // 0x340
    u32 animation_count;  // 0x348
};
static_assert(__builtin_offsetof(UnkHud_42cc, bomb_icon) == 0x298, "UnkHud_42cc layout");
static_assert(__builtin_offsetof(UnkHud_42cc, animation_count) == 0x348, "UnkHud_42cc layout");

// The item-change message: the changed item's id and whether ammo was added.
struct UnkItemChange_42cc {
    base::global::CStrId item;
    u8 pad_0x8[4];
    bool filled;  // 0xc
};

static inline UnkQualifiedId& Qid_42cc(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

static inline UnkGuiDisplayObject* Target(UnkHudRef_42cc* ref) {
    return ref ? ref->target : nullptr;
}

bool fn_71011e42cc(UnkHud_42cc* self, const UnkItemChange_42cc* change) {
    if (self->hidden)
        return false;
    UnkReflObject** end = self->animations + self->animation_count;
    for (UnkReflObject** it = self->animations; it != end; ++it) {
        UnkReflObject* animation = *it;
        if (!animation)
            continue;
        const base::reflection::CClass* type = animation->vf_0x0();
        // The class getter runs for its registration; the class itself is compared.
        fn_71011dd7e0();
        if (fn_710009d50c(type, &g_unk_7172a10410))
            return false;
    }

    if (!(change->item == Qid_42cc(g_unk_717275a818).id) && !(change->item == Qid_42cc(g_unk_717275a258).id) &&
        !(change->item == Qid_42cc(g_unk_717275a298).id))
        return false;
    UnkGuiDisplayObject* label = Target(self->bomb_label);
    UnkGuiDisplayObject* icon = Target(self->bomb_icon);
    if (!label || !icon)
        return false;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;

    s32 current = fn_71011442a0(inventory, Qid_42cc(g_unk_717275a258), 0);
    s32 max = fn_71011442a0(inventory, Qid_42cc(g_unk_717275a298), 0);
    fn_71011cdce8(label, false);
    fn_71011cdce8(icon, false);
    bool owned = fn_71011428bc(inventory, Qid_42cc(g_unk_717275a818));
    if (max < 1 || !owned)
        return false;
    fn_71011cdce8(label, true);
    fn_71011cdce8(icon, true);
    char text[16];
    sprintf(text, "%i / %i", current, max);
    fn_71011ec424(label, CRntString(text));
    label->vf_0x88();
    fn_71011cd5a0(label);
    if (change->filled)
        self->vtable->SendEvent(self, "OnBombFill", false);
    if (current <= 0)
        self->vtable->SendEvent(self, "OnBombEmpty", false);
    return false;
}

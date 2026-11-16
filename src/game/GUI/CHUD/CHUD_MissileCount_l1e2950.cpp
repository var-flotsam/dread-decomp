// GUI::CHUD (unit 2751): on a missile ammo change (current or maximum), unless the HUD is hidden or a
// missile animation is playing, shows "current / max" on the missile counter and fires the "OnMissileFill"
// and "OnMissileEmpty" events. Names and layouts are placeholders.
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

// GUI::CHUD::CMissileAnimation's class (what fn_71011dd568 returns).
__attribute__((visibility("hidden"))) extern base::reflection::CClass g_unk_7172a102d8;

// A reference to a display object of the HUD's layout.
struct UnkHudRef_2950 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
};

struct UnkHud_2950;

struct UnkHudVtable_2950 {
    void* slots[0x3d];
    void (*SendEvent)(UnkHud_2950* self, const char* name, bool flag);  // 0x1e8
};

struct UnkHud_2950 {
    UnkHudVtable_2950* vtable;
    u8 pad_0x8[0x22e];
    bool hidden;  // 0x236
    u8 pad_0x237[0x31];
    UnkHudRef_2950* missile_label;  // 0x268
    u8 pad_0x270[0x10];
    UnkHudRef_2950* missile_icon;  // 0x280
    u8 pad_0x288[0xb8];
    UnkReflObject** animations;  // 0x340
    u32 animation_count;  // 0x348
};
static_assert(__builtin_offsetof(UnkHud_2950, animation_count) == 0x348, "UnkHud_2950 layout");

// The item-change message: the changed item's id and whether ammo was added.
struct UnkItemChange_2950 {
    base::global::CStrId item;
    u8 pad_0x8[4];
    bool filled;  // 0xc
};

static inline UnkQualifiedId& Qid_2950(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline UnkGuiDisplayObject* Target(UnkHudRef_2950* ref) {
    return ref ? ref->target : nullptr;
}

bool fn_71011e2950(UnkHud_2950* self, const UnkItemChange_2950* change) {
    if (self->hidden)
        return false;
    UnkReflObject** end = self->animations + self->animation_count;
    for (UnkReflObject** it = self->animations; it != end; ++it) {
        UnkReflObject* animation = *it;
        if (!animation)
            continue;
        const base::reflection::CClass* type = animation->vf_0x0();
        // The class getter runs for its registration; the class itself is compared.
        fn_71011dd568();
        if (fn_710009d50c(type, &g_unk_7172a102d8))
            return false;
    }

    if (!(change->item == Qid_2950(g_unk_717275a1d8).id) && !(change->item == Qid_2950(g_unk_717275a218).id))
        return false;
    if (!Target(self->missile_label) || !Target(self->missile_icon))
        return false;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;

    s32 current = fn_71011442a0(inventory, Qid_2950(g_unk_717275a1d8), 0);
    s32 max = fn_71011442a0(inventory, Qid_2950(g_unk_717275a218), 0);
    fn_71011cdce8(Target(self->missile_label), false);
    fn_71011cdce8(Target(self->missile_icon), false);
    if (max >= 1) {
        fn_71011cdce8(Target(self->missile_label), true);
        fn_71011cdce8(Target(self->missile_icon), true);
        char text[16];
        sprintf(text, "%i / %i", current, max);
        UnkGuiDisplayObject* label = Target(self->missile_label);
        fn_71011ec424(label, CRntString(text));
        self->missile_label->target->vf_0x88();
        fn_71011cd5a0(Target(self->missile_label));
    }
    if (change->filled)
        self->vtable->SendEvent(self, "OnMissileFill", false);
    if (current <= 0)
        self->vtable->SendEvent(self, "OnMissileEmpty", false);
    return false;
}

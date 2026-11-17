// CLineBombGun's unit (tools/tu_map.py unit 1307): whether a line bomb can be laid, as CBombGun's check: the
// base check, the sub-area GUI cache's count for the tweak track's id, no live line bomb still settling, and
// ITEM_WEAPON_LINE_BOMB in the inventory. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBombGun.h"
#include "game/CBombMovement.h"
#include "game/CGun.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2734.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkInventory;

struct UnkFanBombEntry_ovb0122 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
};

struct UnkFanBombList_ovb0122 {
    UnkFanBombEntry_ovb0122** data;
    u32 count;
};

struct UnkLineBombMovement_l84b078 {
    u8 pad_0x0[0x488];
    bool field_0x488;
    u8 pad_0x489[0xf];
    f32 field_0x498;
};
static_assert(__builtin_offsetof(UnkLineBombMovement_l84b078, field_0x498) == 0x498, "layout");

struct UnkLineBombOwner_l84b078 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkLineBombGun_l84b078 {
    u8 pad_0x0[0x18];
    UnkLineBombOwner_l84b078* owner;  // 0x18
};

static inline UnkQualifiedId& Qid_l84b078(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline UnkSubAreaGuiCache* GuiCache_l84b078() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

// Whether a live line bomb has not yet settled (unset, or its value at +0x498 under 0.2).
static inline bool BombSettling(UnkReflObject* movement) {
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_7100aac8cc()))
        return false;
    UnkLineBombMovement_l84b078* bomb = reinterpret_cast<UnkLineBombMovement_l84b078*>(movement);
    return !bomb->field_0x488 || bomb->field_0x498 < 0.2f;
}

bool fn_710084b078(UnkLineBombGun_l84b078* self) {
    bool base_ok = fn_7100835a14(reinterpret_cast<UnkPrimaryGun_A4_b76*>(self));
    UnkTrackQuery query;
    query.cls = fn_7100c5ce98();
    query.result = nullptr;
    query.value = -1.0f;
    bool no_track = true;
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation) {
            fn_710117b08c(animation, &query);
            no_track = query.result == nullptr;
        }
    }
    UnkSubAreaGuiCache* cache = GuiCache_l84b078();
    const CStrId* name = no_track
        ? reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0x1638)
        : reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(query.result) + 0x18);
    bool counted = fn_710119db04(cache, name) > 0;
    if (!base_ok || !counted)
        return false;
    UnkFanBombList_ovb0122* list = fn_7100aa9ecc();
    bool settled = true;
    UnkFanBombEntry_ovb0122** end = list->data + list->count;
    for (UnkFanBombEntry_ovb0122** it = list->data; it != end; ++it) {
        UnkFanBombEntry_ovb0122* entry = *it;
        if (entry && BombSettling(entry->movement))
            settled = false;
    }
    if (!settled)
        return false;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;
    return fn_71011428bc(inventory, Qid_l84b078(g_unk_717275a7d8));
}

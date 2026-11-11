// CBombGun's unit (tools/tu_map.py): whether a bomb can be dropped (as src/game/CPowerBombGun/CPowerBombGun_CanFire_ovb0130.cpp):
// the base check (fn_7100857344), then the sub-area GUI cache's count for the bomb tweak track's id (or the
// id table's id at +0x980 when the owner's animation has no such track), less the Kraid fight's count
// (+0x149f0 of the Kraid AI the global behaviour-tree manager's actor reports), is above zero and the
// inventory has ITEM_WEAPON_BOMB. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CPrimaryGun.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkSubAreaGuiCache.h"
#include "types.h"

using base::global::CStrId;

struct UnkInventory;
void* fn_7100df55e4(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
// As src/game/CMissileBaseGun/CMissileBaseGun_Ammo_ovb0103.cpp declares them.
extern UnkSubAreaGuiCache* g_unk_71727254f8;
s32 fn_710119db04(UnkSubAreaGuiCache* self, const base::global::CStrId* name);
extern char* g_unk_71726b2398;

// The id table (fn_7100031894 builds it; as src/game/CPowerBombGun/CPowerBombGun_CanFire_ovb0130.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x980];
    CStrId field_0x980;
    u8 pad_0x988[0x2578 - 0x988];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

static inline UnkQualifiedId& Qid_ovb0266(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline UnkSubAreaGuiCache* GuiCache_ovb0266() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

struct UnkBombGunOwner_b0266 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkBombGun_b0266 {
    u8 pad_0x0[0x18];
    UnkBombGunOwner_b0266* owner;  // 0x18
};

struct UnkKraidActor_b0266 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkKraidHolder_b0266 {
    u8 pad_0x0[0x10];
    UnkKraidActor_b0266* actor;  // 0x10
};

bool fn_7100830d38(UnkBombGun_b0266* self) {
    bool base_ok = fn_7100857344(self);
    UnkTrackQuery query;
    query.cls = fn_7100d2d1d8();
    query.result = nullptr;
    query.value = -1.0f;
    bool no_track = true;
    if (self->owner) {
        if (UnkAnimationComponent* animation = self->owner->animation) {
            fn_710117b08c(animation, &query);
            no_track = query.result == nullptr;
        }
    }
    UnkSubAreaGuiCache* cache = GuiCache_ovb0266();
    s32 count;
    if (!no_track)
        count = fn_710119db04(cache, reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(query.result) + 0x18));
    else
        count = fn_710119db04(cache, &GameManagerIdTable().field_0x980);
    if (!base_ok)
        return false;
    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    if (UnkKraidHolder_b0266* holder = *reinterpret_cast<UnkKraidHolder_b0266**>(manager + 0x100)) {
        if (UnkKraidActor_b0266* actor = holder->actor) {
            if (base::reflection::UnkReflObject* ai = actor->ai) {
                if (fn_710009d50c(ai->vf_0x0(), fn_71005fac74()))
                    count = count - *reinterpret_cast<s32*>(reinterpret_cast<char*>(ai) + 0x149f0);
            }
        }
    }
    if (count < 1)
        return false;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;
    return fn_71011428bc(inventory, Qid_ovb0266(g_unk_717275a4d8));
}

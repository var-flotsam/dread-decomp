// CSamusModelUpdaterComponent's unit (tools/tu_map.py unit 1427): when the suit model is pending (+0x219), selects
// the model named for the best suit the inventory holds ("Hyper", "Gravity", "Varia", else "Default"), then
// shows one of two parts by whether Samus holds the central unit energy. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"
#include "../CAccessPointCommanderComponent/UnkStrIdHashedInit.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkInventory;

extern char g_unk_710164b2a0[];
extern char g_unk_710164b2a8[];

struct UnkSamusOwner_l93a82c {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkSamusModelUpdater_l93a82c {
    u8 pad_0x0[0x30];
    UnkSamusOwner_l93a82c* owner;
    u8 pad_0x38[0x219 - 0x38];
    bool suit_pending;  // 0x219
};

// A model name: a string id built from a literal whose length and hash the caller computed
// (CStrId's layout).
struct UnkModelName_l93a82c {
    UnkStringData* data;

    UnkModelName_l93a82c(const char* str, u64 length, u64 hash, bool flag);  // fn_71000406dc
    ~UnkModelName_l93a82c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline bool HasItem_l93a82c(UnkInventory* inventory, UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return fn_71011428bc(inventory, qid);
}

constexpr u64 kHyperHash = StrIdHash("Hyper", 5);
constexpr u64 kGravityHash = StrIdHash("Gravity", 7);
constexpr u64 kVariaHash = StrIdHash("Varia", 5);
constexpr u64 kDefaultHash = StrIdHash("Default", 7);

// The model named for the best suit the inventory holds.
static inline UnkModelName_l93a82c SuitModel_l93a82c(UnkInventory* inventory) {
    if (inventory) {
        if (HasItem_l93a82c(inventory, g_unk_717275a418))
            return UnkModelName_l93a82c("Hyper", 5, kHyperHash, true);
        if (HasItem_l93a82c(inventory, g_unk_717275a458))
            return UnkModelName_l93a82c("Gravity", 7, kGravityHash, true);
        if (HasItem_l93a82c(inventory, g_unk_717275a498))
            return UnkModelName_l93a82c("Varia", 5, kVariaHash, true);
    }
    return UnkModelName_l93a82c("Default", 7, kDefaultHash, true);
}

void fn_710093a82c(UnkSamusModelUpdater_l93a82c* self) {
    if (self->suit_pending) {
        UnkInventory* inventory = self->owner->inventory;
        if (!inventory)
            inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
        UnkModelName_l93a82c model = SuitModel_l93a82c(inventory);
        CMultiModelUpdaterComponent_SelectModelByName(self, reinterpret_cast<const u64*>(&model));
    }
    if (UnkInventory* inventory = self->owner->inventory) {
        bool has_energy = HasItem_l93a82c(inventory, g_unk_717275ac18);
        UnkEnergyWaveFxComp_b49* comp = reinterpret_cast<UnkEnergyWaveFxComp_b49*>(self);
        fn_71011738f8(comp, g_unk_710164b2a0, !has_energy);
        fn_71011738f8(comp, g_unk_710164b2a8, has_energy);
    }
    fn_710093aad0(self);
}

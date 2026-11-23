// CPickableItemComponent's unit: the pickup message. Nothing happens for an item with no message. A tank (its
// kind at +0x170 contains "tank") whose item the player already has, by the id its name gives without "_MAX"
// and "_CURRENT" (else its own ids at +0x98 or +0xa0), shows the item's other message (+0x88); a life shard
// adds one to the player's shards and shows the message for the shard count. The component's slot 0x1a8 is
// told whether it was new and the message is shown. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU112.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

extern void* g_unk_71726ad550;
void* fn_7100ae09a4(void* registry, CStrId id);

f32 fn_7101142988(UnkInventory* inventory, const UnkQualifiedId& item, f32 fallback);

struct UnkPickableItem_l9d0e28;

// What is called back when the message has been shown.
struct UnkPickableCallback_l9d0e28 {
    UnkPickableItem_l9d0e28* item;
    void (*done)();
};
void fn_71009d45cc();
void fn_7100b5fd9c(void* gui, const CStrId* message, const UnkPickableCallback_l9d0e28* callback, u32 a, u32 b);

// The item's description.
struct UnkPickableInfo_l9d0e28 {
    u8 pad_0x0[0x60];
    CStrId message;           // 0x60
    CStrId shardMessages[4];  // 0x68: by the shard count
    CStrId ownedMessage;      // 0x88
};

struct UnkPickableItem_l9d0e28 {
    void** vtable;
    u8 pad_0x8[0x88];
    UnkPickableInfo_l9d0e28* info;  // 0x90
    CStrId itemId;                  // 0x98
    CStrId name;                    // 0xa0
    u8 pad_0xa8[0x81];
    bool checkTank;  // 0x129
    u8 pad_0x12a[0x46];
    CStrId kind;  // 0x170

    void vf_0x1a8(bool isNew) {
        reinterpret_cast<void (*)(UnkPickableItem_l9d0e28*, bool)>(vtable[0x1a8 / 8])(this, isNew);
    }
};
static_assert(__builtin_offsetof(UnkPickableItem_l9d0e28, kind) == 0x170, "item layout");

struct UnkPickableLife_l9d0e28 {
    u8 pad_0x0[0x318];
    f32 shards;  // 0x318
};

struct UnkPickablePlayer_l9d0e28 {
    u8 pad_0x0[0x1c0];
    UnkPickableLife_l9d0e28* life;  // 0x1c0
    u8 pad_0x1c8[0xa8];
    UnkInventory* inventory;  // 0x270
};

struct UnkPickableGuiHolder_l9d0e28 {
    u8 pad_0x0[0x10];
    void* gui;  // 0x10
};

struct UnkPickableGame_l9d0e28 {
    u8 pad_0x0[0x2258];
    UnkPickableGuiHolder_l9d0e28* holder;  // 0x2258
};

static inline bool IsEmpty(const CStrId& id) { return id.data == &g_unk_7101d47108->empty; }

// The id's string, counting the read.
static inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Whether the id's text contains `part` (an empty id contains nothing).
static inline bool Contains(CStrId id, const char* part) {
    ++id.data->reads;
    if (IsEmpty(id))
        return false;
    return CRntString_Contains(&id.data->String(), part);
}

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_l9d0e28(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the player has the item `id` names.
static inline bool Has(UnkInventory* inventory, const CStrId& id) {
    alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
    UnkStrIdHandle& h = *reinterpret_cast<UnkStrIdHandle*>(storage);
    fn_7100ae986c(&h, id);
    bool has = fn_71011428bc(inventory, reinterpret_cast<const UnkQualifiedId&>(h));
    h.id.~CStrId();
    return has;
}

void fn_71009d0e28(UnkPickableItem_l9d0e28* self) {
    if (IsEmpty(self->info->message))
        return;
    auto* player = reinterpret_cast<UnkPickablePlayer_l9d0e28*>(fn_7100dec788(g_unk_7101d47310));
    CStrId message = self->info->message;
    bool isNew = true;
    if (self->checkTank) {
        bool tank = Contains(self->kind, "tank");
        if (tank && player->inventory) {
            UnkInventory* inventory = player->inventory;
            UnkTunableLine line(StringOf(self->name));
            line = fn_710013c834(line.Str(), "_MAX", g_unk_710162e2ed);
            line = fn_710013c834(line.Str(), "_CURRENT", g_unk_710162e2ed);
            // The id the name gives, built in place and destroyed by hand.
            alignas(CStrId) char storage[sizeof(CStrId)];
            CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(storage), *line.Str(), true);
            CStrId& id = *reinterpret_cast<CStrId*>(storage);
            if (!fn_7100ae09a4(g_unk_71726ad550, id)) {
                id = self->itemId;
                if (IsEmpty(id))
                    id = self->name;
            }
            if (!IsEmpty(id) && !Has(inventory, id)) {
                message = self->info->ownedMessage;
                isNew = false;
            }
            id.~CStrId();
        }
    }
    if (CStrId("fLifeShards", true).data == self->itemId.data) {
        if (UnkInventory* inventory = player->inventory) {
            f32 shards = fn_7101142988(inventory, ItemId_l9d0e28(g_unk_717275a0d8), 0.0f) + 1.0f;
            fn_7101142a28(inventory, ItemId_l9d0e28(g_unk_717275a0d8), true, true, true, shards, 0.0f);
        }
        if (UnkPickableLife_l9d0e28* life = player->life) {
            switch ((static_cast<s32>(life->shards) + 1) % 4) {
            case 0:
                message = self->info->shardMessages[3];
                break;
            case 1:
                message = self->info->shardMessages[0];
                break;
            case 2:
                message = self->info->shardMessages[1];
                break;
            case 3:
                message = self->info->shardMessages[2];
                break;
            }
        }
    }
    self->vf_0x1a8(isNew);
    auto* game = reinterpret_cast<UnkPickableGame_l9d0e28*>(g_unk_7101d47310);
    void* gui = game->holder ? game->holder->gui : nullptr;
    UnkPickableCallback_l9d0e28 callback = {self, fn_71009d45cc};
    fn_7100b5fd9c(gui, &message, &callback, 0, 0);
}

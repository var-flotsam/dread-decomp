// CChozoWarriorXUltimateGrabAttack's unit (tools/tu_map.py): sends a value (0 for -1) for an event kind
// with mode 10, first to the owner's (+0x8) character class (+0x220) and, when that applies, to the
// player's (+0x220). Each send picks its own hash for the kind; kinds 0, 1, 2, 5 and 10 send nothing.
// Returns whether both applied. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;
__attribute__((visibility("hidden"))) extern const u64 g_unk_7101649920[];  // the owner's hash per kind
__attribute__((visibility("hidden"))) extern const u64 g_unk_71016498b8[];  // the player's hash per kind

struct UnkUltGrabManagerOwner_488990 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkUltGrabTarget_488990 {
    u8 pad_0x0[0x220];
    void* character;  // 0x220
};

struct UnkUltGrabValue_488990 {
    u64 field_0x0;
    f32 value;
};

struct UnkUltGrab_488990 {
    u8 pad_0x0[0x8];
    UnkUltGrabTarget_488990* owner;  // 0x8
    u8 pad_0x10[0x121];
    bool field_0x131;
};
static_assert(__builtin_offsetof(UnkUltGrab_488990, field_0x131) == 0x131, "layout");

static inline const UnkQualifiedId& MetroidnizationId() {
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_717275ab18);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_717275ab18), g_unk_7101d470e0);
    return qid;
}

static inline bool HasMetroidnization() {
    auto* owner = reinterpret_cast<UnkUltGrabManagerOwner_488990*>(fn_7100defcd4(g_unk_7101d47310));
    UnkInventory* inventory = owner->inventory;
    return inventory && fn_71011428bc(inventory, MetroidnizationId());
}

// The owner's hash for a kind; false for the kinds it does not take.
static inline bool OwnerHash(s32 kind, u64& hash) {
    if (kind == 12 && HasMetroidnization()) {
        hash = 0x3490d5d956677171;
        return true;
    }
    if ((0x427ull >> kind) & 1)
        return false;
    hash = g_unk_7101649920[kind];
    return true;
}

// The player's hash for a kind; false for kinds 0 to 2.
static inline bool PlayerHash(const UnkUltGrab_488990* self, s32 kind, u64& hash) {
    if (kind == 12) {
        if (HasMetroidnization()) {
            hash = 0xacfdc97860b564f3;
            return true;
        }
    } else if (kind == 11) {
        if (self->field_0x131) {
            hash = 0x002efb08d3eb1224;
            return true;
        }
    } else if (kind == 6) {
        if (self->field_0x131) {
            hash = 0x76481b88c94277ac;
            return true;
        }
    } else if ((u32)kind < 3) {
        return false;
    }
    hash = g_unk_71016498b8[kind];
    return true;
}
bool fn_7100488990(UnkUltGrab_488990* self, s32 kind, f32 value) {
    u64 owner_hash, player_hash;
    if (!OwnerHash(kind, owner_hash) || !PlayerHash(self, kind, player_hash))
        return false;    void* owner_target = self->owner->character;
    auto* player = reinterpret_cast<UnkUltGrabTarget_488990*>(fn_7100dec788(g_unk_7101d47310));
    void* player_target = player->character;
    UnkUltGrabValue_488990 param = {0, value != -1.0f ? value : 0.0f};

    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 10;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(owner_target, owner_hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param,
                       &mask))
        return false;

    base::reflection::UnkReflTypeIdArg player_mode;
    player_mode.id = 10;
    base::reflection::UnkReflTypeIdArg player_mask;
    player_mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(player_target, player_hash, 0xf4061ff25a2163d6, &player_mode, &g_unk_710164b968,
                         &param, &player_mask);
}

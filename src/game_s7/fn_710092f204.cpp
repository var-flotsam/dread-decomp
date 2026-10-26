// Picks the hit effect of a melee attack: by attack kind (2, 5 or other) and by the owner's
// suit (from its inventory), or the plain one while it is in some state. Layouts are
// placeholders; the item globals are the item definitions of fn_710107a880.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkSuitProperties.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// An item definition (game/UnkGameBlackboard.h): its qualified id is built on first use.
struct UnkItemDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

static inline const UnkQualifiedId& GetQualifiedId(UnkItemDef& def) {
    if ((u64)def.qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}


struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

using UnkStateSet = UnkAnimationComponent;

struct UnkOwner {
    u8 unk_0x0[0x220];
    UnkStateSet* states;
    u8 unk_0x228[0x48];
    UnkInventory* inventory;
};

struct UnkMelee {
    u8 unk_0x0[0x30];
    UnkOwner* owner;
};

// NON_MATCHING(minor): the original tests kind 5 before kind 2, keeps a copy of the selection per
// kind, and moves each item id's first-use build out of line (duplicating the inventory test);
// the same statics are built and the same one is returned.
const CFilePathStrId& fn_710092f204(UnkMelee* self, s32 kind) {
    static const CFilePathStrId s_0("system/fx/hits/melee.bcptl", true);
    static const CFilePathStrId s_1("system/fx/hits/meleesuc.bcptl", true);
    static const CFilePathStrId s_2("system/fx/hits/meleesuc.bcptl", true);
    static const CFilePathStrId s_3("system/fx/hits/melee_gravity.bcptl", true);
    static const CFilePathStrId s_4("system/fx/hits/meleesuc_gravity.bcptl", true);
    static const CFilePathStrId s_5("system/fx/hits/meleesuc_gravity.bcptl", true);
    static const CFilePathStrId s_6("system/fx/hits/melee_gravity.bcptl", true);
    static const CFilePathStrId s_7("system/fx/hits/meleesuc_gravity.bcptl", true);
    static const CFilePathStrId s_8("system/fx/hits/meleesuc_gravity.bcptl", true);
    static const CFilePathStrId s_9("system/fx/hits/melee_varia.bcptl", true);
    static const CFilePathStrId s_10("system/fx/hits/meleesuc_varia.bcptl", true);
    static const CFilePathStrId s_11("system/fx/hits/meleesuc_varia.bcptl", true);
    static const CFilePathStrId s_12("system/fx/hits/melee_power.bcptl", true);
    static const CFilePathStrId s_13("system/fx/hits/meleesuc_power.bcptl", true);
    static const CFilePathStrId s_14("system/fx/hits/meleesuc_power.bcptl", true);
    bool plain = true;
    if (UnkStateSet* states = self->owner->states)
        plain = !fn_710117b348(states, fn_7100c13dd8());
    bool a = false, b = false, c = false;
    if (self->owner->inventory) {
        a = fn_71011428bc(self->owner->inventory, GetQualifiedId(SuitAs<UnkItemDef>(g_unk_717275a418)));
        b = a || fn_71011428bc(self->owner->inventory, GetQualifiedId(SuitAs<UnkItemDef>(g_unk_717275a458)));
        c = b || fn_71011428bc(self->owner->inventory, GetQualifiedId(SuitAs<UnkItemDef>(g_unk_717275a498)));
    }
    if (kind == 5) {
        if (plain || a)
            return plain ? s_2 : s_5;
        return b ? s_8 : c ? s_11 : s_14;
    }
    if (kind == 2) {
        if (plain || a)
            return plain ? s_1 : s_4;
        return b ? s_7 : c ? s_10 : s_13;
    }
    if (plain || a)
        return plain ? s_0 : s_3;
    return b ? s_6 : c ? s_9 : s_12;
}

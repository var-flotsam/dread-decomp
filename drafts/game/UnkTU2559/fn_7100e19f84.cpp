// Unit 2559: how much danger the player is in, as a level from 0 to 3: an active E.M.M.I. (1 for a
// proto or a flagged one, else 2), central unit energy in the inventory (1 once it decays, else 2), a
// boss fight (3), or an active central unit (1 or 3). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU500.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// The objects fn_7100552d7c lists (as src/game/CBreakableTileGroup.cpp sees them): a component at +0x10.
struct UnkListedItem {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};

struct UnkCentralUnitEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
};

struct UnkCentralUnitList {
    UnkCentralUnitEntry** data;
    u32 count;
};

bool fn_7100775348(char* p0);

struct UnkEmmyAI_le19f84 {
    u8 pad_0x0[0x27720];
    bool field_0x27720;
};

struct UnkCentralUnitState_le19f84 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkCentralUnit_le19f84 {
    u8 pad_0x0[0x40];
    UnkCentralUnitState_le19f84* field_0x40;
};

struct UnkBossActor_le19f84 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkBossHolder_le19f84 {
    u8 pad_0x0[0x10];
    UnkBossActor_le19f84* actor;  // 0x10
};

struct UnkGameManagerView_le19f84 {
    u8 pad_0x0[0xab0];
    CBlackboard field_0xab0;
};

// Whether the object is of the class (or derives from it) that the getter returns.
template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsA_le19f84(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), GetClass());
}

// Reads the float `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadFloat_le19f84(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def,
                                     f32* out) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395b34(s, &qid.id, out);
}

s32 fn_7100e19f84() {
    base::global::CRntVector<UnkListedItem*>* items = fn_7100552d7c();
    for (UnkListedItem **it = items->data, **end = items->data + items->count; it != end; ++it) {
        if (!*it)
            continue;
        UnkReflObject* component = (*it)->field_0x10;
        if (!component)
            continue;
        if (IsA_le19f84<fn_710053b934>(component) && fn_710024fbc8(component)) {
            if (!reinterpret_cast<UnkEmmyAI_le19f84*>(component)->field_0x27720 &&
                !IsA_le19f84<fn_71004fa488>(component))
                return 2;
            return 1;
        }
    }

    CBlackboard* blackboard = &reinterpret_cast<UnkGameManagerView_le19f84*>(g_unk_7101d47310)->field_0xab0;
    static CStrId s_inventory("PLAYER_INVENTORY", true);
    f32 energy = 0.0f;
    ReadFloat_le19f84(blackboard, s_inventory, g_unk_717275ac18, &energy);
    if (energy > 0.0f) {
        f32 decayed = 0.0f;
        ReadFloat_le19f84(blackboard, s_inventory, g_unk_717275ac58, &decayed);
        return decayed > 0.0f ? 1 : 2;
    }

    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    if (UnkBossHolder_le19f84* holder = *reinterpret_cast<UnkBossHolder_le19f84**>(manager + 0x100)) {
        if (UnkBossActor_le19f84* actor = holder->actor) {
            if (UnkReflObject* ai = actor->ai) {
                if (IsA_le19f84<fn_71003ab524>(ai))
                    return 3;
            }
        }
    }

    UnkCentralUnitList* units = fn_7100772c98();
    for (UnkCentralUnitEntry **it = units->data, **end = units->data + units->count; it != end; ++it) {
        if (!*it)
            continue;
        UnkReflObject* object = (*it)->object;
        if (!object)
            continue;
        UnkCentralUnitState_le19f84* state = reinterpret_cast<UnkCentralUnit_le19f84*>(object)->field_0x40;
        if (!state || (state->state != 3 && state->state != 4))
            continue;
        return fn_7100775348(reinterpret_cast<char*>(object)) ? 1 : 3;
    }
    return 0;
}

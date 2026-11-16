// Unit 1493 (CPickableItemComponent): whether the archive of a scenario is locked: reads the
// "ArchivesLocked" flags from a blackboard section and picks the scenario's (locked when it has
// none). Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2564.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using unk_static_init::UnkEnumType;

extern UnkPropertyEx<> g_unk_717275adf0;  // "ArchivesLocked"
extern UnkEnumType g_unk_7101d60e78;      // the flags' type

// The flags read from the section are converted to the flags' type.
struct UnkArchiveFlagsTag;
template <>
struct base::reflection::UnkFieldTraits<UnkArchiveFlagsTag> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_7101d60e78); }
};

// One flag per scenario; freed on destruction.
struct UnkFlagList_l9d2dc0 : CRntVector<bool> {
    UnkFlagList_l9d2dc0() : CRntVector<bool>(0xf) {}
    ~UnkFlagList_l9d2dc0() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The id of the section that holds the flags (a slot of the game's id table).
static inline const CStrId& FlagsSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

static inline bool SameId(const base::global::UnkStrIdBase& a, const base::global::UnkStrIdBase& b) {
    return a.data == b.data;
}

static inline s32 ScenarioIndex(const CStrId& scenario) {
    if (SameId(scenario, g_unk_71727c7fe8))
        return 0;
    if (SameId(scenario, g_unk_71727c7ff0))
        return 1;
    if (SameId(scenario, g_unk_71727c7ff8))
        return 2;
    if (SameId(scenario, g_unk_71727c8000))
        return 3;
    if (SameId(scenario, g_unk_71727c8008))
        return 4;
    if (SameId(scenario, g_unk_71727c8010))
        return 5;
    if (SameId(scenario, g_unk_71727c8018))
        return 6;
    if (SameId(scenario, g_unk_71727c8020))
        return 7;
    return 0;
}

bool fn_71009d2dc0(const CStrId& scenario) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x820;
    UnkFlagList_l9d2dc0 locked;
    const CStrId& section_id = FlagsSectionId();
    const UnkQualifiedId& name = Qid(g_unk_717275adf0);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        base::refl_GetKeyedValue<UnkArchiveFlagsTag>(reinterpret_cast<char*>(section),
                                                     reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                                                     &locked);
    s32 index = ScenarioIndex(scenario);
    if (static_cast<s32>(locked.count) > index)
        return locked.data[index];
    return true;
}

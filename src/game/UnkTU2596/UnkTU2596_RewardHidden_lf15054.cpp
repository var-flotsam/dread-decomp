// Unit 2596: whether an ending reward ("0_OK" to "1_EXCELLENT") is still hidden: reads the hidden
// flags from the profile blackboard and picks the reward's (hidden when it has none). Names and
// layouts are placeholders.
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
#include "game/UnkTU1645.h"
#include "game/UnkTU2564.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using unk_static_init::UnkEnumType;

extern UnkEnumType g_unk_7101d60e78;      // the flags' type

// The flags read from the section are converted to the flags' type.
struct UnkRewardFlagsTag_lf15054;
template <>
struct base::reflection::UnkFieldTraits<UnkRewardFlagsTag_lf15054> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_7101d60e78); }
};

// One flag per reward; freed on destruction.
struct UnkFlagList_lf15054 : CRntVector<bool> {
    UnkFlagList_lf15054() : CRntVector<bool>(0xf) {}
    ~UnkFlagList_lf15054() {
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
static inline const CStrId& RewardsSectionId() {
    return GameManagerIdTable().field_0x1bf0;
}

static inline s32 RewardIndex(const CStrId& reward) {
    if (reward == CStrId("0_OK", true))
        return 0;
    if (reward == CStrId("0_GOOD", true))
        return 1;
    if (reward == CStrId("0_EXCELLENT", true))
        return 2;
    if (reward == CStrId("1_OK", true))
        return 3;
    if (reward == CStrId("1_GOOD", true))
        return 4;
    if (reward == CStrId("1_EXCELLENT", true))
        return 5;
    return 0;
}

bool fn_7100f15054(CStrId* reward) {
    CBlackboard* profile = &g_unk_7101d47310->field_0x820;
    UnkFlagList_lf15054 hidden;
    const CStrId& section_id = RewardsSectionId();
    const UnkQualifiedId& name = Qid(g_unk_717275aeb8);
    if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
        base::refl_GetKeyedValue<UnkRewardFlagsTag_lf15054>(reinterpret_cast<char*>(section),
                                                         reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                                                         &hidden);
    s32 index = RewardIndex(*reward);
    if (static_cast<s32>(hidden.count) > index)
        return hidden.data[index];
    return true;
}

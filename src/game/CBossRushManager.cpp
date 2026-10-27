// From unit 2544 of tools/tu_map.py (0x7100db464c-0x7100dd1e34): CBossRushManager's unit (its
// static initialiser is src/game_s7/fn_7100dd14a0.cpp; the manager's set-up, which ends with the
// function below, is src/tables/boss_rush_7100db8dec.cpp). Reading the manager's records from the
// user data: the boss rush and dread rush records (CBossRushManager::SRushRecord, by reflection),
// the survival rush record and its time. Names and layouts are placeholders.
#include "../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using unk_static_init::UnkEnumType;

// The user data's owner, created on first use (as src/game_aca57c/fn_7100dc8828.cpp has it).
struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);  // constructor
CBlackboard* fn_710120384c(UnkUserDataOwner* self);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// The unit's properties ("BossRushRecord", "DreadRushRecord", "SurvivalRushRecord",
// "SurvivalRushTime"), the records' type (CBossRushManager::SRushRecord) and the id of the user
// data's section that holds them (one of the unit's static ids).
extern UnkPropertyEx<> g_unk_71727bca40;
extern UnkPropertyEx<> g_unk_71727bca80;
extern UnkPropertyEx<> g_unk_71727bcac0;
extern UnkPropertyEx<> g_unk_71727bcb00;
extern UnkEnumType g_unk_71727bcb40;
extern const CStrId g_unk_71727bdb80 __attribute__((visibility("hidden")));

// A record read from the section is converted to the record type.
struct UnkRushRecordTag;
template <>
struct base::reflection::UnkFieldTraits<UnkRushRecordTag> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_71727bcb40); }
};

// Read an int and a float from a section (src/clones/: refl_GetKeyedValue instances).
bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
bool fn_7100395b34(CBlackboard::CSection* section, const void* name, f32* out);

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// A CBossRushManager::SRushRecord (0xa8 bytes; its members are not needed here).
struct UnkBossRushRecord {
    u8 data[0xa8];
};

// The manager as this reader sees it (the set-up's layout, UnkBossRush, names fewer members).
struct UnkBossRushManager {
    u8 pad_0x0[0xa68];
    UnkBossRushRecord boss_rush_record;   // "BossRushRecord"
    UnkBossRushRecord dread_rush_record;  // "DreadRushRecord"
    u8 pad_0xbb8[0xa8];
    s32 survival_rush_record;  // "SurvivalRushRecord"
    f32 survival_rush_time;    // "SurvivalRushTime"
};
static_assert(__builtin_offsetof(UnkBossRushManager, survival_rush_record) == 0xc60, "UnkBossRushManager layout");

// Reads the record `item` names into `out` (left as it is when the section or the value is
// missing).
static inline void ReadRecord(CBlackboard* data, UnkPropertyEx<>& item, UnkBossRushRecord* out) {
    const UnkQualifiedId& name = Qid(item);
    if (CBlackboard::CSection* section = fn_7101047270(data, g_unk_71727bdb80))
        base::refl_GetKeyedValue<UnkRushRecordTag>(reinterpret_cast<char*>(section),
                                                   reinterpret_cast<const base::UnkStrIdMember*>(&name.id), out);
}

struct UnkBossRush;

// Loads the records from the user data: the survival record defaults to -1 and its time to 0.
void fn_7100dc82b4(UnkBossRush* manager) {
    UnkBossRushManager* self = reinterpret_cast<UnkBossRushManager*>(manager);
    CBlackboard* data = fn_710120384c(GetOwner());
    ReadRecord(data, g_unk_71727bca40, &self->boss_rush_record);
    ReadRecord(data, g_unk_71727bca80, &self->dread_rush_record);
    self->survival_rush_record = -1;
    {
        const UnkQualifiedId& name = Qid(g_unk_71727bcac0);
        if (CBlackboard::CSection* section = fn_7101047270(data, g_unk_71727bdb80))
            fn_7100395a30(section, &name, &self->survival_rush_record);
    }
    self->survival_rush_time = 0.0f;
    {
        const UnkQualifiedId& name = Qid(g_unk_71727bcb00);
        if (CBlackboard::CSection* section = fn_7101047270(data, g_unk_71727bdb80))
            fn_7100395b34(section, &name, &self->survival_rush_time);
    }
}

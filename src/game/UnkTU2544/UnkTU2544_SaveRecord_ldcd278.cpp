// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): records the level times of a finished
// boss rush or dread rush, keeps them as the best times when they beat the record, and saves the record to
// the user data. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2544.h"
#include "game/UnkTU2768.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::UnkStrIdBase;
using unk_static_init::UnkEnumType;

using UnkKeyedWords = base::global::CRntSmallDictionary<u32, u32>;
u32* fn_7100dd0b9c(UnkKeyedWords* self, const u32* key, const u32* value);

// "BossRushRecord", "DreadRushRecord", the records' type (CBossRushManager::SRushRecord) and the two flags
// set when a record is saved.
extern UnkPropertyEx<> g_unk_71727bca40;
extern UnkPropertyEx<> g_unk_71727bca80;
extern UnkEnumType g_unk_71727bcb40;

// The user data's owner, created on first use.
struct UnkUserDataOwner;

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

using UnkLevelTimes = base::global::CRntSmallDictionary<u32, f32>;

// A CBossRushManager::SRushRecord: the time of each level, the best times and the run count they took.
struct UnkRushRecord_dcd278 {
    UnkLevelTimes times;       // 0x0
    UnkLevelTimes best_times;  // 0x28
    u8 pad_0x50[0xa0 - 0x50];
    s32 field_0xa0;
    u8 pad_0xa4[4];
};

struct UnkRushLevelTime_dcd278 {
    f32 time;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkRushSurvival_dcd278 {
    u8 pad_0x0[0xb9];
    bool dread_rush;  // 0xb9
    u8 pad_0xba[0x998 - 0xba];
    UnkRushLevelTime_dcd278 levels[13];  // 0x998
    u8 pad_0xa34[4];
    f64 time;                            // 0xa38
    u8 pad_0xa40[0xa68 - 0xa40];
    UnkRushRecord_dcd278 boss_rush_record;   // 0xa68
    UnkRushRecord_dcd278 dread_rush_record;  // 0xb10
    u8 pad_0xbb8[0xc68 - 0xbb8];
    bool new_record;  // 0xc68
    u8 pad_0xc69[0xc74 - 0xc69];
    s32 runs;  // 0xc74
};
static_assert(__builtin_offsetof(UnkRushSurvival_dcd278, time) == 0xa38, "layout");
static_assert(__builtin_offsetof(UnkRushSurvival_dcd278, runs) == 0xc74, "layout");

static inline void AddTime(UnkLevelTimes* times, u32 level, f32* time) {
    fn_7100dd0b9c(reinterpret_cast<UnkKeyedWords*>(times), &level, reinterpret_cast<const u32*>(time));
}

static inline void StoreRecord(CBlackboard* data, UnkPropertyEx<>& item, UnkRushRecord_dcd278* record) {
    UnkQualifiedId& name = Qid(item);
    if (CBlackboard::CSection* section = fn_71010472e0(data, g_unk_71727bdb80)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = reinterpret_cast<const base::reflection::CType*>(&g_unk_71727bcb40);
            v.value = record;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

static inline void StoreFlag(CBlackboard* data, UnkPropertyName* name) {
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(data, g_unk_71727bdb80)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(section, name, &v);
        }
    }
}

void fn_7100dcd278(UnkRushSurvival_dcd278* self) {
    UnkRushRecord_dcd278* record = self->dread_rush ? &self->dread_rush_record : &self->boss_rush_record;
    self->new_record = true;
    if (record->best_times.count != 0) {
        f32 t = (f32)self->time;
        f32 time = t > 0.0f ? __builtin_floorf(t) : __builtin_ceilf(t);
        bool better = time < record->best_times.End()[-1];
        self->new_record = better;
        if (better)
            record->best_times.count = 0;
    }
    record->times.count = 0;
    for (u32 level = 1; level <= 12; ++level) {
        AddTime(&record->times, level, &self->levels[level].time);
        if (self->new_record)
            AddTime(&record->best_times, level, &self->levels[level].time);
    }
    if (self->new_record)
        record->field_0xa0 = self->runs;
    CBlackboard* data = fn_710120384c(GetOwner());
    if (self->dread_rush) {
        StoreRecord(data, g_unk_71727bca80, &self->dread_rush_record);
        StoreFlag(data, reinterpret_cast<UnkPropertyName*>(&g_unk_71727bd290));
    } else {
        StoreRecord(data, g_unk_71727bca40, &self->boss_rush_record);
        StoreFlag(data, reinterpret_cast<UnkPropertyName*>(&g_unk_71727bd278));
    }
    fn_7101203854(GetOwner());
}

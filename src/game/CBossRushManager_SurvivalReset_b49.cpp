// From unit 2544 of tools/tu_map.py: CBossRushManager's unit. The writing counterpart of the survival
// rush reader in src/game/CBossRushManager.cpp (fn_7100dc82b4): resets the survival record (-1) and
// its time (0), stores both in the user data's section (unless the data is locked), then tells the
// user data's owner (fn_7101203854). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkUserDataOwner;
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);  // constructor
CBlackboard* fn_710120384c(UnkUserDataOwner* self);
void fn_7101203854(UnkUserDataOwner* self);

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// "SurvivalRushRecord", "SurvivalRushTime" and the section's id (as CBossRushManager.cpp has them).
extern UnkPropertyEx<> g_unk_71727bcac0;
extern UnkPropertyEx<> g_unk_71727bcb00;
extern const CStrId g_unk_71727bdb80 __attribute__((visibility("hidden")));

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

template <const base::reflection::CType* (*Type)()>
__attribute__((always_inline)) static inline void Store(CBlackboard* data, UnkPropertyEx<>& item, void* value) {
    UnkQualifiedId& name = Qid(item);
    if (CBlackboard::CSection* section = fn_71010472e0(data, g_unk_71727bdb80)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = Type();
            v.value = value;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

struct UnkBossRushSurvival {
    u8 pad_0x0[0xc60];
    s32 survival_rush_record;  // "SurvivalRushRecord"
    f32 survival_rush_time;    // "SurvivalRushTime"
};

void fn_7100dcdfa4(UnkBossRushSurvival* self) {
    self->survival_rush_record = -1;
    self->survival_rush_time = 0.0f;
    CBlackboard* data = fn_710120384c(GetOwner());
    Store<fn_7100014768>(data, g_unk_71727bcac0, &self->survival_rush_record);
    Store<fn_710000abbc>(data, g_unk_71727bcb00, &self->survival_rush_time);
    fn_7101203854(GetOwner());
}

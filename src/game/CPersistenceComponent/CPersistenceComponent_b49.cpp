// CPersistenceComponent unit (tools/tu_map.py): fn_71009ca384 restores the owner's entity state from
// a blackboard. The section is the definition's own (+0x58 of +0x40) or, when that is the empty id,
// the current scenario's; the entity (+0x30) is switched off (fn_7100232864) when the definition's bool
// key (+0x60) holds true, its int key (+0x70) a positive value or its float key (+0x80) a positive value,
// tried in that order and skipping empty keys. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

bool fn_7100395a30(CBlackboard::CSection*, const void*, s32*);
bool fn_7100395b34(CBlackboard::CSection*, const void*, f32*);
struct UnkEntityState;

struct UnkPersistenceKey_b49 {
    CStrId id;
    u64 field_0x8;

    bool IsSet() const { return id.data != &g_unk_7101d47108->empty; }
};

struct UnkPersistenceDef_b49 {
    u8 pad_0x0[0x58];
    CStrId section;
    UnkPersistenceKey_b49 bool_key;
    UnkPersistenceKey_b49 int_key;
    UnkPersistenceKey_b49 float_key;
};

struct UnkPersistence_b49 {
    u8 pad_0x0[0x30];
    UnkEntityState* entity;
    u8 pad_0x38[0x8];
    UnkPersistenceDef_b49* def;
};

static inline CStrId SectionId(UnkPersistenceDef_b49* def) {
    if (def->section.data == g_unk_7101d470e0.data)
        return fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    return def->section;
}

static inline bool BoolSet(CBlackboard::CSection* section, const UnkPersistenceKey_b49& key, bool* flag) {
    if (!key.IsSet())
        return false;
    if (!fn_710006d11c(section, &key.id, flag))
        return false;
    return *flag;
}

static inline bool IntPositive(CBlackboard::CSection* section, const UnkPersistenceKey_b49& key) {
    if (!key.IsSet())
        return false;
    s32 value = 0;
    return fn_7100395a30(section, &key.id, &value) && value > 0;
}

static inline bool FloatPositive(CBlackboard::CSection* section, const UnkPersistenceKey_b49& key) {
    if (!key.IsSet())
        return false;
    f32 value = 0.0f;
    return fn_7100395b34(section, &key.id, &value) && value > 0.0f;
}

void fn_71009ca384(UnkPersistence_b49* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, SectionId(self->def));
    if (!section)
        return;
    bool flag = false;
    if (BoolSet(section, self->def->bool_key, &flag) ||
        IntPositive(section, self->def->int_key) || FloatPositive(section, self->def->float_key))
        fn_7100232864(self->entity, false);
}

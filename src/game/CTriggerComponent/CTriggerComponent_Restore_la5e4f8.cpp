// CTriggerComponent unit (tools/tu_map.py): fn_7100a5e4f8 restores the trigger's saved state from the
// blackboard: the unit's flag (g_unk_717274d6c8) from the current section, and each condition's done flag
// (g_unk_717274d6f0, qualified by the trigger's name) from the scenario's section. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkTriggerCondition_a5e4f8 {
    CStrId name;
    u8 pad_0x8[0x50];
    bool done;  // 0x58
};

struct UnkTriggerValue_a5e4f8 {
    CStrId id;
    u64 field_0x8;
};

struct UnkTriggerRestore_a5e4f8 {
    u8 pad_0x0[0x30];
    UnkEntityState* entity;  // 0x30
    u8 pad_0x38[0x218];
    UnkTriggerCondition_a5e4f8** conditions;  // 0x250
    u32 condition_count;                      // 0x258
    u8 pad_0x25c[0x1c];
    bool enabled;  // 0x278
    u8 pad_0x279[0xf];
    CStrId name;                       // 0x288
    UnkTriggerValue_a5e4f8* values;    // 0x290
    u64* keys;                         // 0x298
    u32 count;                         // 0x2a0
    u8 pad_0x2a4[0x14];
    UnkQualifiedName condition_name;   // 0x2b8
    u8 pad_0x388[0x90];
    bool flag;  // 0x418
};
static_assert(__builtin_offsetof(UnkTriggerRestore_a5e4f8, condition_name) == 0x2b8, "");
static_assert(__builtin_offsetof(UnkTriggerRestore_a5e4f8, flag) == 0x418, "");

static inline bool GetSectionBool(CBlackboard* blackboard, const CStrId& section, const CStrId* name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, name, out);
    return false;
}

// Builds the qualified name of the conditions' done flag on first use.
static inline void BuildConditionName(UnkTriggerRestore_a5e4f8* self) {
    if (!self->condition_name.str || !self->condition_name.str[0])
        fn_7100ae9d7c(&self->condition_name, (UnkIdSource*)&g_unk_717274d6f0, CStrId(self->name));
}

void fn_7100a5e4f8(UnkTriggerRestore_a5e4f8* self) {
    if (!self->enabled)
        return;
    bool value = self->flag;
    CStrId section_id = fn_7100de9c30(g_unk_7101d47310);
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717274d6c8.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkTriggerValue_a5e4f8* values = self->values;
    if (GetSectionBool(blackboard, section_id, &values[index].id, &value))
        fn_7100232864(self->entity, value);

    CStrId scenario_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    UnkTriggerCondition_a5e4f8** end = self->conditions + self->condition_count;
    CBlackboard* scenario_blackboard = &g_unk_7101d47310->field_0x968;
    for (UnkTriggerCondition_a5e4f8** it = self->conditions; it != end; ++it) {
        BuildConditionName(self);
        const UnkSubId_7100fc4b34& id = fn_71003327a4(&self->condition_name, &(*it)->name);
        GetSectionBool(scenario_blackboard, scenario_id, &id.id, &(*it)->done);
    }
}

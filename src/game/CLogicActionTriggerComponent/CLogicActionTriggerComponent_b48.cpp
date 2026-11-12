// CLogicActionTriggerComponent unit (tools/tu_map.py): the trigger's "Enabled" flag (g_unk_7172735488, the
// LA_TRIGGER property) in the blackboard section of the current scenario, its name looked up in the key list
// at +0x80 (count +0x88, values at +0x78) as in CEventScenarioComponent_b46.cpp. fn_710090f720 reads it
// from the given blackboard (creating the section) and, when found, switches the entity at +0x18 to it
// (fn_7100232864); fn_710090f894 and fn_710090fa80 store true and false unless the blackboard is locked (the
// setter as in CDoorLifeComponent.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CLogicActionTriggerComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntityState;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkLogicActionTriggerValue_b48 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkLogicActionTrigger_b48 {
    u8 pad_0x0[0x18];
    UnkEntityState* entity;
    u8 pad_0x20[0x58];
    UnkLogicActionTriggerValue_b48* values;
    u64* keys;
    u32 count;

    // The index of the "Enabled" value (one past the last when there is none).
    __attribute__((always_inline)) u64 EnabledIndex() {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)g_unk_7172735488.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return index;
    }

    __attribute__((always_inline)) void StoreEnabled(bool* value, bool enabled) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = EnabledIndex();
        UnkLogicActionTriggerValue_b48* v = values;
        *value = enabled;
        SetSectionBool(blackboard, section_id, &v[index].name, value);
    }
};

void fn_710090f720(UnkLogicActionTrigger_b48* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    bool enabled = false;
    u64 index = self->EnabledIndex();
    if (fn_710006d11c(section, &self->values[index].id, &enabled))
        fn_7100232864(self->entity, enabled);
}

void fn_710090f894(UnkLogicActionTrigger_b48* self) {
    bool value;
    self->StoreEnabled(&value, true);
}

void fn_710090fa80(UnkLogicActionTrigger_b48* self) {
    bool value;
    self->StoreEnabled(&value, false);
}

// CDoorShieldLifeComponent unit (tools/tu_map.py): fn_71008de9d4 stores true under the shield's "Dead"
// property (g_unk_7172732320, CItemLifeComponent.cpp) in the blackboard section of the current scenario
// unless the blackboard is locked: the name looked up in the key list at +0x270 (count +0x278, values at
// +0x268) as in CEventScenarioComponent_b46.cpp, the setter as in CDoorLifeComponent.cpp. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CItemLifeComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

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

struct UnkDoorShieldValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkDoorShieldLife_b48 {
    u8 pad_0x0[0x268];
    UnkDoorShieldValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_71008de9d4(UnkDoorShieldLife_b48* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    bool dead;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172732320.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkDoorShieldValue_b48* values = self->values;
    dead = true;
    SetSectionBool(blackboard, section_id, &values[index].name, &dead);
}

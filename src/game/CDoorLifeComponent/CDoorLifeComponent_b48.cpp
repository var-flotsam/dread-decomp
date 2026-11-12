// CDoorLifeComponent unit (tools/tu_map.py): fn_71008d8890 sets "ControlledByUsable" (+0xaef) and stores
// it in the blackboard section of the current scenario unless the blackboard is locked: the property's
// name looked up in the key list at +0x678 (count +0x680, values at +0x670) as in
// CEventScenarioComponent_b46.cpp, the setter as in CDoorLifeComponent.cpp. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
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

struct UnkDoorLifeValue_b48 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkDoorLife_b48 {
    u8 pad_0x0[0x670];
    UnkDoorLifeValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0x684[0xaef - 0x684];
    bool controlled_by_usable;
};

void fn_71008d8890(UnkDoorLife_b48* self, bool controlled) {
    bool value = controlled;
    self->controlled_by_usable = controlled;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172731188.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkDoorLifeValue_b48* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
}

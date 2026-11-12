// CTriggerComponent unit (tools/tu_map.py): fn_7100a61790 stores a flag under the unit's property
// (g_unk_717274d6c8) in the current scenario's blackboard section (fn_7100de9c30), while the component
// is enabled (+0x278) and the blackboard is neither locked nor busy (fn_7100ded930); the property's
// name is looked up in the key list at +0x298 (count +0x2a0, values +0x290) as in
// CCharClassActivatableComponent_b48.cpp. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

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

struct UnkTriggerValue_ovb0067 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkTriggerSaved_ovb0067 {
    u8 pad_0x0[0x278];
    bool field_0x278;
    u8 pad_0x279[0x17];
    UnkTriggerValue_ovb0067* values;  // 0x290
    u64* keys;                        // 0x298
    u32 count;                        // 0x2a0
};

// Stores the flag under the unit's property (g_unk_717274d6c8) in the current scenario's blackboard
// section, while the object is enabled (+0x278) and the blackboard is neither locked nor busy.
void fn_7100a61790(UnkTriggerSaved_ovb0067* self, bool flag) {
    bool value = flag;
    if (!self->field_0x278)
        return;
    if (g_unk_7101d47310->field_0x968.field_0xe8 || fn_7100ded930(g_unk_7101d47310))
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_7100de9c30(manager);
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
    UnkTriggerValue_ovb0067* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
}

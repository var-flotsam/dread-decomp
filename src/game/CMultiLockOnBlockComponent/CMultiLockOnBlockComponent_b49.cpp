// CMultiLockOnBlockComponent unit (tools/tu_map.py): fn_71008834d4 stores the block's flag (the property
// g_unk_717272b638, keys at +0x88, count +0x90, values at +0x80) in the current scenario's blackboard
// section (the empty id without a scenario), but only when that section exists already or can be made
// (fn_71010472e0, as in CMultiLockOnBlockComponent_b48.cpp); the setter as in
// src/game/CDoorLifeComponent.cpp. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CMultiLockOnBlockComponent.h"
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

struct UnkMultiLockOnValue_b49 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkMultiLockOnBlock_b49 {
    u8 pad_0x0[0x80];
    UnkMultiLockOnValue_b49* values;
    u64* keys;
    u32 count;
};

void fn_71008834d4(UnkMultiLockOnBlock_b49* self, u32 flag) {
    UnkGameManager* manager = g_unk_7101d47310;
    UnkGameManagerSub_0x608* scenario = manager->field_0x608;
    CBlackboard* blackboard = &manager->field_0x968;
    if (!fn_71010472e0(blackboard, scenario ? fn_71011a2fbc(scenario) : CStrId(g_unk_7101d470e0)))
        return;
    UnkGameManagerSub_0x608* current = g_unk_7101d47310->field_0x608;
    CStrId section_id = current ? fn_71011a2fbc(current) : CStrId(g_unk_7101d470e0);
    bool value;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717272b638.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkMultiLockOnValue_b49* values = self->values;
    value = flag != 0;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
}

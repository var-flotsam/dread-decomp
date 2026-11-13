// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101119b7c stores the group's flag (+0x138)
// in the given blackboard, in the section of the current scenario, under its property (the name looked up
// in the key list at +0x3f8, count +0x400, values at +0x3f0), unless the blackboard is locked. The shape of
// CLogicActionTriggerComponent_b48.cpp's fn_710090f894; the setter as in CDoorLifeComponent.cpp. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172a065e0;

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkSpawnGroupValue_b49 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkSpawnGroup_b49 {
    u8 pad_0x0[0x138];
    bool flag;
    u8 pad_0x139[0x2b7];
    UnkSpawnGroupValue_b49* values;
    u64* keys;
    u32 count;
};

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

void fn_7101119b7c(UnkSpawnGroup_b49* self, CBlackboard* blackboard) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 n = self->count;
    if (n) {
        u64* k = self->keys;
        u64 key = (u64)g_unk_7172a065e0.name.data;
        for (; index != n; ++index) {
            if (k[index] == key)
                break;
        }
    }
    bool value;
    bool flag = self->flag;
    UnkSpawnGroupValue_b49* v = self->values;
    value = flag;
    SetSectionBool(blackboard, section_id, &v[index].name, &value);
}

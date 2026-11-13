// CCharClassEventScenarioComponent unit (tools/tu_map.py): fn_71007e41b8 finishes the event: clears
// +0x1d8, sets +0x1ee, runs fn_71007e2ecc once if the event had not finished yet (its last argument
// 0.25 unless +0x1e4 is set), marks it finished (+0x1ed) and, when +0x1e4 says so, stores the flag
// under the "EventFinished" property in the current scenario's blackboard section (the lookup of
// CEventScenarioComponent_b46.cpp; the setter as in src/game/CDoorLifeComponent.cpp). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

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

struct UnkEventScenarioValue_b49a2 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkEventScenario_b49a2 {
    u8 pad_0x0[0xb0];
    UnkEventScenarioValue_b49a2* values;
    u64* keys;
    u32 count;
    u8 pad_0xc4[0x1d8 - 0xc4];
    u32 field_0x1d8;
    u8 pad_0x1dc[0x1e4 - 0x1dc];
    bool field_0x1e4;
    u8 pad_0x1e5[0x1ed - 0x1e5];
    bool finished;
    bool field_0x1ee;
};

void fn_71007e41b8(UnkEventScenario_b49a2* self) {
    self->field_0x1d8 = 0;
    self->field_0x1ee = true;
    if (!self->finished)
        fn_71007e2ecc(self, true, false, 0.0f, 0.0f, 0.0f, self->field_0x1e4 ? 0.0f : 0.25f);
    self->finished = true;
    if (self->field_0x1e4) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 n = self->count;
        if (n) {
            u64* k = self->keys;
            u64 key = (u64)g_unk_717271f350.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        UnkEventScenarioValue_b49a2* v = self->values;
        SetSectionBool(blackboard, section_id, &v[index].name, &self->finished);
    }
}

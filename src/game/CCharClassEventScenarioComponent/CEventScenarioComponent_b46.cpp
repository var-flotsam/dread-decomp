// An event scenario component reading its "EventFinished" flag (tools/tu_map.py: the
// CCharClassEventScenarioComponent unit): looks the property's name up in the component's key list
// (+0xb8, count +0xc0; the values, 16 bytes each, at +0xb0) and, when the game manager's blackboard
// has the section named by the current scenario id, reads the flag into +0x1ed. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkEventScenarioValue_b46a2 {
    CStrId id;
    u64 field_0x8;
};

struct UnkEventScenario_b46a2 {
    u8 pad_0x0[0xb0];
    UnkEventScenarioValue_b46a2* values;
    u64* keys;
    u32 count;
    u8 pad_0xc4[0x1ed - 0xc4];
    bool finished;
};

void fn_71007e15d0(UnkEventScenario_b46a2* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_717271f350.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkEventScenarioValue_b46a2* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_710006d11c(section, &values[index].id, &self->finished);
}

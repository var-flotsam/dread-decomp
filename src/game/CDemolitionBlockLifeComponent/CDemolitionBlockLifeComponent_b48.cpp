// CDemolitionBlockLifeComponent unit (tools/tu_map.py): fn_71008c5970 reads the block's flag
// (g_unk_7172730c98) from the blackboard section of the current scenario, its name looked up in the key list
// at +0x270 (count +0x278, values at +0x268) as in CEventScenarioComponent_b46.cpp. False when it is not
// found. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172730c98;

struct UnkDemolitionBlockValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkDemolitionBlockLife_b48 {
    u8 pad_0x0[0x268];
    UnkDemolitionBlockValue_b48* values;
    u64* keys;
    u32 count;
};

bool fn_71008c5970(UnkDemolitionBlockLife_b48* self) {
    bool value = false;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)g_unk_7172730c98.name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkDemolitionBlockValue_b48* values = self->values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_710006d11c(section, &values[index].id, &value);
    }
    return value;
}

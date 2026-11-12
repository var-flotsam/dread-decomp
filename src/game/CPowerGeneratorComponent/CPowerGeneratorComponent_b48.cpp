// CPowerGeneratorComponent unit (tools/tu_map.py): fn_7100a7e8a0 reads the generator's "Used" flag
// (the shape of CEventScenarioComponent_b46.cpp's fn_71007e15d0): clears +0x138, then, with a current
// scenario (+0x608 of the game manager), looks the property's name up in the component's key list (+0x98,
// count +0xa0; the values, 16 bytes each, at +0x90) and, when the game manager's blackboard has the
// section named by the scenario id, reads the flag into +0x138. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CPowerGeneratorComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkPowerGeneratorValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkPowerGenerator_b48 {
    u8 pad_0x0[0x90];
    UnkPowerGeneratorValue_b48* values;
    u64* keys;
    u32 count;
    u8 pad_0xa4[0x138 - 0xa4];
    bool used;
};

void fn_7100a7e8a0(UnkPowerGenerator_b48* self) {
    self->used = false;
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_71727502f8.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkPowerGeneratorValue_b48* values = self->values;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_710006d11c(section, &values[index].id, &self->used);
}

// CChozoCommanderXLifeComponent unit (tools/tu_map.py): fn_71008bda40 reads the "Dead" flag from the
// blackboard section of the current scenario (fn_71010472e0 creates it when missing; the property's name
// looked up in the key list at +0x290, count +0x298, values at +0x288, as in
// CEventScenarioComponent_b46.cpp) and, when it is found, switches the entity at +0x18 on unless dead
// (fn_7100232864). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CChozoCommanderXLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

struct UnkChozoCommanderXValue_b48 {
    CStrId id;
    u64 field_0x8;
};

struct UnkChozoCommanderXLife_b48 {
    u8 pad_0x0[0x18];
    UnkEntityState* entity;
    u8 pad_0x20[0x268];
    UnkChozoCommanderXValue_b48* values;
    u64* keys;
    u32 count;
};

void fn_71008bda40(UnkChozoCommanderXLife_b48* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, fn_71011a2fbc(manager->field_0x608));
    if (!section)
        return;
    bool dead = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172730210.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    if (fn_710006d11c(section, &self->values[index].id, &dead))
        fn_7100232864(self->entity, !dead);
}

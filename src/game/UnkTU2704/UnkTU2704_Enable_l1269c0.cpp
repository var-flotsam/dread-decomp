// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): enables the component and its owner entity and, when
// automanaged, stores true for its "Enabled" property in the current scenario's blackboard section.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkPropertyName;

// The slot of the "Enabled" property among the component's keys; the key count when it is not there.
static inline u64 IndexOfKey(CSpawnPointComponent* self) {
    u64 count = self->field_0x2a0;
    for (u64 i = 0; i != count; i++) {
        if (self->field_0x298[i] == g_unk_7172a06908.name.data)
            return i;
    }
    return count;
}

bool fn_71011269c0(CSpawnPointComponent* self) {
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), true);
    fn_7100232864(static_cast<UnkEntityState*>(self->field_0x18), true);
    if (self->bAutomanaged) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkTypedValue typed;
        bool value;
        CBlackboard* blackboard = &manager->field_0x968;
        CBlackboard::CSection* section;
        {
            CStrId scenario = fn_71011a2fbc(manager->field_0x608);
            section = fn_71010472e0(blackboard, scenario);
        }
        if (section) {
            u64 index = IndexOfKey(self);
            u8* entries = static_cast<u8*>(self->field_0x290);
            UnkPropertyName* name = reinterpret_cast<UnkPropertyName*>(entries + index * 16);
            value = true;
            if (!blackboard->field_0xe8) {
                typed.type = fn_710000d228();
                typed.value = &value;
                fn_7100332c80(section, name, &typed);
            }
        }
    }
    return true;
}

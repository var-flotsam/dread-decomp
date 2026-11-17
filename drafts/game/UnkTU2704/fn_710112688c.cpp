// CSpawnPointComponent unit: when the component is automanaged, reads its "Enabled" flag from the current
// scenario's blackboard section and enables or disables the component by that flag. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTU2704.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172a06908;

void fn_710112688c(CSpawnPointComponent* self, CBlackboard* blackboard) {
    if (!self->bAutomanaged)
        return;
    bool enabled = false;
    bool found;
    {
        CStrId scenario = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        // The slot of the "Enabled" property among the component's keys; the key count when it is not there.
        u64 index = 0;
        u32 count = self->field_0x290.count;
        if (count) {
            for (; index != count; ++index) {
                if (self->field_0x290.keys[index].data == g_unk_7172a06908.name.data)
                    break;
            }
        }
        UnkStrIdTableValue* values = self->field_0x290.values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, scenario))
            found = fn_710006d11c(section, &values[index].id, &enabled);
        else
            found = false;
    }
    if (found) {
        if (enabled)
            fn_71011269c0(self);
        else
            fn_7101126af4(self);
    }
}

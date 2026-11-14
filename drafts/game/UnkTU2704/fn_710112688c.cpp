// CSpawnPointComponent unit: when the component is automanaged, reads its "Enabled" flag from the current
// scenario's blackboard section and calls one of two handlers by that flag. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTU2704.h"

using base::global::CStrId;

extern UnkProperty g_unk_7172a06908;
void fn_71011269c0(void* self);

struct UnkSpawnPointEntry_l12688c {
    CStrId id;
    u64 field_0x8;
};

void fn_710112688c(CSpawnPointComponent* self, CBlackboard* blackboard) {
    if (!self->bAutomanaged)
        return;
    bool flag = false;
    u32 found;
    {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 count = self->field_0x2a0;
    if (count) {
        u64 key = (u64)g_unk_7172a06908.name.data;
        for (; index != count; ++index) {
            if (self->field_0x298[index] == key)
                break;
        }
    }
    UnkSpawnPointEntry_l12688c* entries = static_cast<UnkSpawnPointEntry_l12688c*>(self->field_0x290);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        found = fn_710006d11c(section, &entries[index].id, &flag);
    else
        found = 0;
    }
    if (found & 1) {
        if (flag)
            fn_71011269c0(self);
        else
            fn_7101126af4(self);
    }
}

// CShelmitAIComponent unit (tools/tu_map.py). fn_71006bc188 gives the id for an event: the empty id,
// or for event 12 the unit's id (g_unk_71727071d8) when the owner's (+0x30) animation component
// (+0x220) is in state 12 (fn_7101178e6c). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_b35.cpp declares it.
struct UnkTakumakuComponent;
bool fn_7101178e6c(UnkTakumakuComponent* component, u32 kind);

extern CStrId g_unk_71727071d8 __attribute__((visibility("hidden")));

struct UnkShelmitOwner_b50 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* field_0x220;
};

struct UnkShelmitAI_b50 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_b50* owner;
};

void fn_71006bc188(UnkShelmitAI_b50* self, void* arg, u32 event, CStrId* out) {
    *out = g_unk_7101d470e0;
    if (event == 12) {
        if (UnkTakumakuComponent* component = self->owner->field_0x220) {
            if (fn_7101178e6c(component, 12))
                *out = g_unk_71727071d8;
        }
    }
}

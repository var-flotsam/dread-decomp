// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py unit 2566): two handlers that,
// when the owner's scenario id (+0x110, as fn_7100e90e6c in src/game/CMinimapManager.cpp reads it) is
// set and is the game manager's current one (fn_7100de9bf8), pass the argument to the object at +0x118
// (fn_7100e791a4) with kinds 6 and 4. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

const base::global::CStrId* fn_7100de9bf8(UnkGameManager* self);
void fn_7100e791a4(void* self, void* arg, u32 kind, u32 a, void* b);

struct UnkMinimapScenarioOwner_b55 {
    u8 unk_0x0[0x110];
    base::global::CStrId field_0x110;
    u8 field_0x118[0x8];
};

template <u32 Kind>
static inline void NotifyIfCurrent_b55(UnkMinimapScenarioOwner_b55* self, void* arg) {
    if (self->field_0x110.data == &g_unk_7101d47108->empty)
        return;
    const base::global::CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
    if (self->field_0x110.data != current->data)
        return;
    fn_7100e791a4(self->field_0x118, arg, Kind, 0, nullptr);
}

void fn_7100e95038(UnkMinimapScenarioOwner_b55* self, void* arg) { NotifyIfCurrent_b55<6>(self, arg); }
void fn_7100e950b4(UnkMinimapScenarioOwner_b55* self, void* arg) { NotifyIfCurrent_b55<4>(self, arg); }

// CWeightActivablePropComponent unit (tools/tu_map.py, unit 1626): restores the prop from the blackboard: reads
// "Opened" (g_unk_7172755b08) from the section the game manager names (fn_7100de9c30) and requests state 2 from
// the component's state machine at +0x130 ("none" 3) when it is set. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_7100de9c30(UnkGameManager* manager);
extern UnkProperty g_unk_7172755b08;  // "Opened"

struct UnkPropValue_A5_b73 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkPropRestore_A5_b73 {
    u8 pad_0x0[0x130];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x1e0[0x210 - 0x1e0];
    UnkPropValue_A5_b73* values;
    u64* keys;
    u32 count;
};
static_assert(__builtin_offsetof(UnkPropRestore_A5_b73, values) == 0x210, "layout");

void fn_7100ac9714(UnkPropRestore_A5_b73* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_7100de9c30(g_unk_7101d47310));
    if (!section)
        return;
    bool opened = false;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172755b08.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    bool found = fn_710006d11c(section, reinterpret_cast<const CStrId*>(&self->values[index].name), &opened);
    if (opened && found)
        RequestState_A5_b73<15, 3>(&self->machine, 2);
}

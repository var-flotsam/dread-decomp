// CHeatRoomComponent's unit (tools/tu_map.py unit 1538): after the base step (fn_7100a45cac), takes the
// current heat parameters from the alternative set when the game manager's flag at +0x2441 is up (else
// the normal set), keeping the first value when that set was already applied, hands the sound system the
// room's sound path and refreshes the room (fn_7100a26828). Names and layouts are placeholders.
#include "base/snd/CSoundSystem.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkHeatCurrent_ovb0083.h"
#include "game/UnkHeatPair_ovb0083.h"
#include "game/UnkHeatParams_ovb0083.h"
#include "game/UnkTU23.h"
#include "types.h"

// Takes a parameter set; the first value only when the set was not applied before.
inline void ApplyHeat_la26758(UnkHeatCurrent_ovb0083& current, const UnkHeatParams_ovb0083& params, bool wasSet) {
    if (!wasSet)
        current.field_0x0 = params.field_0x0;
    current.field_0x4 = params.field_0x4;
    current.field_0x8.x = params.field_0x8.x;
    current.field_0x8.y = params.field_0x8.y;
    current.field_0x10 = params.field_0x10;
    current.field_0x14 = params.field_0x18;
}

struct UnkHeatRoom_la26758 {
    u8 pad_0x0[0xbc0];
    UnkHeatCurrent_ovb0083 current;  // 0xbc0
    u8 pad_0xbd8[0xc];
    UnkHeatParams_ovb0083 normal;  // 0xbe4
    UnkHeatParams_ovb0083 alt;     // 0xc04
    u32 use_alt;                   // 0xc24
    u8 pad_0xc28[0x28];
    u8 sound_path[0x8];  // 0xc50
    u8 pad_0xc58[0x28];
    bool normal_was_set;  // 0xc80
    bool alt_was_set;     // 0xc81
};
static_assert(__builtin_offsetof(UnkHeatRoom_la26758, use_alt) == 0xc24, "UnkHeatRoom_la26758 layout");
static_assert(__builtin_offsetof(UnkHeatRoom_la26758, sound_path) == 0xc50, "UnkHeatRoom_la26758 layout");
static_assert(__builtin_offsetof(UnkHeatRoom_la26758, normal_was_set) == 0xc80, "UnkHeatRoom_la26758 layout");

void CHeatRoomComponent_ApplyConfigAndPassEnterZoneSound(UnkHeatRoom_la26758* self) {
    fn_7100a45cac(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
    u32 useAlt = reinterpret_cast<u8*>(g_unk_7101d47310)[0x2441];
    self->use_alt = useAlt;
    if (useAlt)
        ApplyHeat_la26758(self->current, self->alt, self->alt_was_set);
    else
        ApplyHeat_la26758(self->current, self->normal, self->normal_was_set);
    fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode_CEntity_2*>(self->sound_path));
    self->normal_was_set = self->normal.field_0x1c;
    self->alt_was_set = self->alt.field_0x1c;
    fn_7100a26828(reinterpret_cast<UnkHeatRoom_b49*>(self));
}

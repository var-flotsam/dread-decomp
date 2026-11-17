// CHeatRoomComponent's unit (tools/tu_map.py unit 1538): after the base step (fn_7100a45cac), takes the
// current heat parameters from the alternative set when the game manager's flag at +0x2441 is up (else
// the normal set), keeping the first value when that set was already applied, hands the sound system the
// room's sound path and refreshes the room (fn_7100a26828). Names and layouts are placeholders.
#include "base/snd/CSoundSystem.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU23.h"
#include "types.h"

struct UnkHeatPair_ovb0083 {
    f32 x;
    f32 y;
};

struct UnkHeatParams_ovb0083 {
    f32 field_0x0;
    f32 field_0x4;
    UnkHeatPair_ovb0083 field_0x8;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    bool field_0x1c;
};

struct UnkHeatCurrent_ovb0083 {
    f32 field_0x0;
    f32 field_0x4;
    UnkHeatPair_ovb0083 field_0x8;
    f32 field_0x10;
    f32 field_0x14;

    // Takes a parameter set; the first value only when the set was not applied before.
    void Apply(const UnkHeatParams_ovb0083& params, bool wasSet) {
        if (!wasSet)
            field_0x0 = params.field_0x0;
        field_0x4 = params.field_0x4;
        field_0x8.x = params.field_0x8.x;
        field_0x8.y = params.field_0x8.y;
        field_0x10 = params.field_0x10;
        field_0x14 = params.field_0x18;
    }
};

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

void fn_7100a26758(UnkHeatRoom_la26758* self) {
    fn_7100a45cac(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
    u32 useAlt = reinterpret_cast<u8*>(g_unk_7101d47310)[0x2441];
    self->use_alt = useAlt;
    if (useAlt)
        self->current.Apply(self->alt, self->alt_was_set);
    else
        self->current.Apply(self->normal, self->normal_was_set);
    fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(self->sound_path));
    self->normal_was_set = self->normal.field_0x1c;
    self->alt_was_set = self->alt.field_0x1c;
    fn_7100a26828(reinterpret_cast<UnkHeatRoom_b49*>(self));
}

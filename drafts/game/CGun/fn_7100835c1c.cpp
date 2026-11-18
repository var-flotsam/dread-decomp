// CGun unit (tools/tu_map.py unit 1302): when the gun is active (+0x78), `kind` is not 2, the key at
// +0x80 is set and `skip` is clear, plays the gun's entry (+0x1874) in the manager g_unk_71726af0b0
// (fn_7100ed6a10), then gives the actor (+0x18) the timeline key at +0x98 unless it is -1
// (fn_7100d7c828). Names and layouts are placeholders.
#include "game/CAbilityEnergyWave.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkShotManager_b0011;
UnkSoundHandle fn_7100ed6a10(UnkShotManager_b0011* self, u32 type, u32 variant);

struct UnkGun_l835c1c {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x58];
    bool active;  // 0x78
    u8 pad_0x79[0x7];
    u64 key_check;  // 0x80
    u8 pad_0x88[0x10];
    u64 key;  // 0x98
    u8 pad_0xa0[0x17d4];
    u32 id;  // 0x1874
};
static_assert(__builtin_offsetof(UnkGun_l835c1c, id) == 0x1874, "UnkGun_l835c1c placeholder layout");

void fn_7100835c1c(UnkGun_l835c1c* self, s32 kind, bool skip) {
    if (self->active && kind != 2 && self->key_check != 0xffffffffffffffffull && !skip) {
        fn_7100ed6a10(static_cast<UnkShotManager_b0011*>(g_unk_71726af0b0), self->id, 0);
        if (self->key != 0xffffffffffffffffull)
            fn_7100d7c828(self->actor, &self->key, false);
    }
}

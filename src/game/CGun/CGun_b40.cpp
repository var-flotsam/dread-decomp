// CGun unit (tools/tu_map.py unit 1302): fn_7100835d38, when the gun is active (+0x78), resets its
// entry (+0x1874) in the manager g_unk_71726af0b0 for the three slots 0, 2 and 1 (fn_7100ed6af0), then
// gives the actor (+0x18) the timeline key at +0xb0 or +0xa8 (chosen by `second`) unless it is -1
// (fn_7100d7c828). Names and layouts are placeholders.
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern void* g_unk_71726af0b0;

struct UnkGun_gn {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x58];
    bool active;  // 0x78
    u8 pad_0x79[0x2f];
    u64 key_a;  // 0xa8
    u64 key_b;  // 0xb0
    u8 pad_0xb8[0x17bc];
    u32 id;  // 0x1874
};

void fn_7100835d38(UnkGun_gn* self, void* unused, bool second) {
    if (!self->active)
        return;
    fn_7100ed6af0(g_unk_71726af0b0, self->id, 0);
    fn_7100ed6af0(g_unk_71726af0b0, self->id, 2);
    fn_7100ed6af0(g_unk_71726af0b0, self->id, 1);
    const u64* key;
    if (second) {
        key = &self->key_b;
        if (*key == 0xffffffffffffffffull)
            return;
    } else {
        key = &self->key_a;
        if (*key == 0xffffffffffffffffull)
            return;
    }
    fn_7100d7c828(self->actor, key, false);
}

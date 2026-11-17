// base::snd::CSoundSystem's unit (tools/tu_map.py): updates the member at +0x8 (fn_71000fa028), then
// takes each group flag from the global settings (g_unk_7108b6c5f0 from +0x21) as fn_7100107eb8 sets
// one: a flag turned off also clears its group in the member. Names and layouts are placeholders.
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkSoundSettings_ovb0074 {
    u8 pad_0x0[0x21];
    bool flags[8];  // 0x21
};

char* fn_71000fa028(char* p0, f32 f0);

struct UnkSoundSystemUpdate_l107bac {
    void** vtable;
    char member[0x400910 - 0x8];  // 0x8
    bool flags[8];                // 0x400910
};

// Takes group `index`'s flag from the global settings.
static inline void SyncGroup(UnkSoundSystemUpdate_l107bac* self, s32 index) {
    bool current = self->flags[index];
    bool on = g_unk_7108b6c5f0.flags[index];
    if (current == on)
        return;
    self->flags[index] = on;
    if (!on) {
        void* member = self->member;
        u32 mask = 1u << index;
        fn_71000fea74(member, mask, 0.0f);
        alignas(8) u32 bits = mask;
        fn_71000fd1d4(member, &bits, 0.0f);
    }
    g_unk_7108b6c5f0.flags[index] = on;
}

void fn_7100107bac(UnkSoundSystemUpdate_l107bac* self, f32 dt) {
    fn_71000fa028(self->member, dt);
    SyncGroup(self, 0);
    SyncGroup(self, 6);
    SyncGroup(self, 1);
    SyncGroup(self, 2);
    SyncGroup(self, 3);
    SyncGroup(self, 4);
    SyncGroup(self, 5);
    SyncGroup(self, 7);
}

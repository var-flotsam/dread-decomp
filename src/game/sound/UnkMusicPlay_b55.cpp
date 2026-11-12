// Unit 2627 of tools/tu_map.py and its neighbours (the units of fn_7100fce86c's callbacks,
// src/game_7100ece7fc/fn_7100fce86c.cpp): while the id at 0x2238 of the game manager is set, a
// music request is played through the request object (fn_7100fd147c with the object's value at
// +0x10 and the unit's callback), unless it is flagged at +0x30 and fn_7100fd1ad8 takes it first;
// otherwise the music manager singleton (created on first use) gets fn_7100fdb9e0. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2636.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkMgr_0xf18;

struct UnkMusicRequest_b55 {
    u8 unk_0x0[0x10];
    u32 field_0x10;
    u8 unk_0x14[0x1c];
    u8 field_0x30;
};

void fn_7100fd147c(UnkMusicRequest_b55* self, u32 id, u32 value, void (*callback)());

void fn_7100fce7dc();
void fn_7100fcf28c();
void fn_7100fcf6bc();
void fn_7100fcf9cc();

template <void (*Callback)()>
static inline void PlayMusic_b55(UnkMusicRequest_b55* self, u32 id) {
    if (g_unk_7101d47310->field_0x2238.data != &g_unk_7101d47108->empty) {
        if (self->field_0x30 && fn_7100fd1ad8(self, 0))
            return;
        fn_7100fd147c(self, id, self->field_0x10, Callback);
        return;
    }
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    fn_7100fdb9e0(static_cast<UnkMgr_0xf18*>(instance), 1, 0, -1.0f, 0.0f);
}

void fn_7100fce918(UnkMusicRequest_b55* self, u32 id) { PlayMusic_b55<fn_7100fce7dc>(self, id); }
void fn_7100fcf3c8(UnkMusicRequest_b55* self, u32 id) { PlayMusic_b55<fn_7100fcf28c>(self, id); }
void fn_7100fcf7f8(UnkMusicRequest_b55* self, u32 id) { PlayMusic_b55<fn_7100fcf6bc>(self, id); }
void fn_7100fcfb08(UnkMusicRequest_b55* self, u32 id) { PlayMusic_b55<fn_7100fcf9cc>(self, id); }

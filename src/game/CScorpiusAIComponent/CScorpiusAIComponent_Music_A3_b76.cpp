// CScorpiusAIComponent's unit (tools/tu_map.py), fn_7100698200: makes two calls of fn_7100fdcf2c on the music
// manager (game/sound/CMusicManager.h) with 0.2, their flags set by the flag at +0x14c28, then sets the flag at
// +0x14d80. Names and layouts are placeholders.
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

// The music manager singleton (as src/game/CHydrogigaAIComponent/CHydrogigaAIComponent_Send_b59_2.cpp declares it).
extern UnkMgr_0xf18* g_unk_71726bc560;

static inline void* MusicManager_A3_b76() {
    if (!reinterpret_cast<void*&>(g_unk_71726bc560)) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        reinterpret_cast<void*&>(g_unk_71726bc560) = m;
    }
    return reinterpret_cast<void*&>(g_unk_71726bc560);
}

struct UnkScorpiusMusic_A3_b76 {
    bool flag;  // 0x14c28
    u8 pad_0x1[0x158 - 0x1];
    bool done;  // 0x14d80
};

struct UnkScorpiusMusicAI_A3_b76 {
    u8 pad_0x0[0x14c28];
    UnkScorpiusMusic_A3_b76 music;  // 0x14c28
};

void fn_7100698200(UnkScorpiusMusicAI_A3_b76* self) {
    UnkScorpiusMusic_A3_b76* music = &self->music;
    if (music->flag) {
        fn_7100fdcf2c(MusicManager_A3_b76(), 0, 1, 0.2f);
        fn_7100fdcf2c(MusicManager_A3_b76(), 1, 0, 0.2f);
    } else {
        fn_7100fdcf2c(MusicManager_A3_b76(), 0, 0, 0.2f);
        fn_7100fdcf2c(MusicManager_A3_b76(), 1, 1, 0.2f);
    }
    music->done = true;
}

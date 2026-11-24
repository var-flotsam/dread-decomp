// CCharClassFadeOutMusicEvent's unit (tools/tu_map.py): the event's run. The music manager
// (g_unk_71726bc560, created on first use, game/sound/CMusicManager.h) fades out (fn_7100fd7b44)
// channel 2 on every layer over the event's time (+0xc). The mask types are
// src/game/UnkLoadingScreen.cpp's. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "game/UnkMask.h"

// The music manager, created on first use.
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

struct UnkFadeOutMusicEvent_fom {
    u8 pad_0x0[0xc];
    f32 time;  // 0xc
};

void CCharClassFadeOutMusicEvent_FadeOutMusicOverTime(UnkFadeOutMusicEvent_fom* self) {
    void* manager = MusicManager();
    f32 time = self->time;
    CTunableMusicManager_StopMusicOnAllSlots(static_cast<UnkMusicMgr_b0019*>(manager), UnkMask(2),
                  UnkMaskOf<UnkMusicMaskTag>::All(), time);
}

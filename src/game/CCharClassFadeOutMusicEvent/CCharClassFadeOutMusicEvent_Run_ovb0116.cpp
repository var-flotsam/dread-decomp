// CCharClassFadeOutMusicEvent's unit (tools/tu_map.py): the event's run. The music manager
// (g_unk_71726bc560, created on first use, game/sound/CMusicManager.h) fades out (fn_7100fd7b44)
// channel 2 on every layer over the event's time (+0xc). The mask types are
// src/game/UnkLoadingScreen.cpp's. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "base/UnkLoadingMask.h"

// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy), and its two
// guarded constants.
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;

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

void fn_7100cbb1a4(UnkFadeOutMusicEvent_fom* self) {
    void* manager = MusicManager();
    f32 time = self->time;
    fn_7100fd7b44(manager, UnkLoadingMask(2), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), time);
}

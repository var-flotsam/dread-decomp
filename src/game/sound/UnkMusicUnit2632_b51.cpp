// Unit 2632 (tools/tu_map.py; no reflected class): asks the music manager (g_unk_71726bc560, created on
// first use, game/sound/CMusicManager.h) for fn_7100fd7b44 with the values 10 and 0x40 and -1.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "game/UnkMask.h"

void fn_7100fcf59c() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    CTunableMusicManager_StopMusicOnAllSlots(static_cast<UnkMusicMgr_b0019*>(instance), UnkMask(10), UnkMask(0x40), -1.0f);
}
